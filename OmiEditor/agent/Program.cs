using System;
using System.IO;
using System.Net;
using System.Linq;
using System.Threading;
using System.Diagnostics;
using System.Net.Sockets;
using System.IO.Compression;
using System.Collections.Generic;
using System.Runtime.Serialization.Formatters.Binary;

using DBHelper;
using Library;

namespace agent
{
    internal static class Program
    {
        private static string _currentline = string.Empty;

        private static readonly ManualResetEvent Stop = new ManualResetEvent(false);

        private static void Exec(string cmd)
        {
            if (cmd.Length <= 0) return;

            switch (cmd)
            {
                case "newpatch":
                {
                    var di = new DirectoryInfo("newpatch");

                    if (di.Exists)
                    {
                        Patcher.AddNewPatch(di);
                        Patcher.RestartAll();
                    }
                    else WriteLine("newpatch directory does not exit");
                    break;
                }
                case "forcepatch":
                {
                    var di = new DirectoryInfo("newpatch");

                    if (di.Exists)
                    {
                        Patcher.AddNewPatch(di, true);
                        Patcher.RestartAll();
                    }
                    else WriteLine("newpatch directory does not exit");
                    break;
                }
                case "restartall":
                    Patcher.RestartAll();
                    break;
                case "killall":
                    Patcher.KillAll();
                    break;
                default:
                    if (cmd.StartsWith("setversion:")) Patcher.ChangeVersion(Patcher.StrToVer(cmd.Substring("setversion:".Length)));

                    else if (cmd.StartsWith("tcpserver:"))
                    {
                        bool opt;

                        if (bool.TryParse(cmd.Substring("tcpserver:".Length), out opt))
                        {
                            WriteLine("TCP server is " + (opt ? "enabled." : "disabled."));
                            Server.AcceptPatches = opt;
                        }
                    }

                    else if (cmd == "version") WriteLine(Patcher.VerToStr(Patcher.Version));

                    else WriteLine("Doing nothing, unknown command.");
                    break;
            }
        }

        public static void WriteLine(string line)
        {
            lock (_currentline)
            {
                // Are there any characters to erase? Clear them all
                if (_currentline.Length > 0)
                {
                    // Determine where we are in the console buffer.
                    var oldLength = _currentline.Length;
                    var oldRows = oldLength / 80;

                    Console.CursorLeft = 0;

                    Console.CursorTop = Console.CursorTop - oldRows;

                    Console.Write(new string(' ', oldLength + 2));

                    Console.CursorLeft = 0;

                    Console.CursorTop = Console.CursorTop - oldRows;

                }
                else
                {
                    Console.CursorLeft = 0;
                    Console.Write("  ");
                    Console.CursorLeft = 0;
                }

                Console.WriteLine(line);

                Console.Write("> ");
                Console.Write(_currentline);
            }

        }

        private static void Main()
        {
            WriteLine("Current version is: " + Patcher.VerToStr(Patcher.Version));

            new Thread(() => HttpStuff.Listen(@"http://10.10.20.6:11080/")).Start();

            new Thread(Server.StartListening).Start();
            

            // Configure console.
            //Console.BufferWidth = 80;
            //Console.WindowWidth = 80;
            Console.TreatControlCAsInput = true;

            // Console.WriteLine("Enter a string. Press <Enter> or Esc to exit.");
            while (!Stop.WaitOne(20))
            {
                if (_currentline == string.Empty && Console.CursorLeft == 0) Console.Write("> ");

                if (Console.KeyAvailable)
                {
                    string cmd = null;

                    ConsoleKeyInfo keyInfo;
                    lock (_currentline)
                    {
                        keyInfo = Console.ReadKey(true);
                        // Ignore if Alt or Ctrl is pressed.
                        if ((keyInfo.Modifiers & ConsoleModifiers.Alt) == ConsoleModifiers.Alt)
                            continue;
                        if ((keyInfo.Modifiers & ConsoleModifiers.Control) == ConsoleModifiers.Control)
                            continue;
                        // Ignore if KeyChar value is \u0000.
                        if (keyInfo.KeyChar == '\u0000') continue;
                        // Ignore tab key.
                        if (keyInfo.Key == ConsoleKey.Tab) continue;
                        // Handle backspace.
                        if (keyInfo.Key == ConsoleKey.Backspace)
                        {
                            // Are there any characters to erase?
                            if (_currentline.Length > 0)
                            {
                                // Determine where we are in the console buffer.
                                var cursorCol = Console.CursorLeft - 1;
                                var oldLength = _currentline.Length;
                                var extraRows = (oldLength + 2) / Console.BufferWidth;

                                _currentline = _currentline.Substring(0, oldLength - 1);
                                Console.CursorLeft = 0;
                                Console.CursorTop = Console.CursorTop - extraRows;
                                Console.Write("> ");
                                Console.Write(_currentline);
                                Console.Write(" ");

                                if (cursorCol >= 0) Console.CursorLeft = cursorCol;
                                else
                                {
                                    --Console.CursorTop;
                                    Console.CursorLeft = Console.BufferWidth - 1;
                                }
                            }
                            continue;
                        }
                        // Handle Escape key.
                        if (keyInfo.Key == ConsoleKey.Escape)
                        {
                            // Are there any characters to erase? Erase them all.
                            if (_currentline.Length > 0)
                            {
                                // Determine where we are in the console buffer.
                                var oldLength = _currentline.Length;
                                var extraRows = (oldLength + 2) / Console.BufferWidth;

                                Console.CursorLeft = 0;
                                Console.CursorTop = Console.CursorTop - extraRows;

                                Console.Write("> ");
                                Console.Write(new string(' ', oldLength));

                                Console.CursorTop = Console.CursorTop - extraRows;
                                Console.CursorLeft = 2;
                            }

                            _currentline = string.Empty;

                            continue;
                        }

                        if (keyInfo.Key == ConsoleKey.Enter)
                        {
                            cmd = _currentline;
                            _currentline = string.Empty;
                            Console.WriteLine();
                        }
                        else
                        {
                            // Handle key by adding it to input string.
                            Console.Write(keyInfo.KeyChar);
                            _currentline += keyInfo.KeyChar;
                        }
                    }

                    if (keyInfo.Key == ConsoleKey.Enter)
                    {
                        Exec(cmd);

                        _currentline = string.Empty;
                    }
                }
            }
        }
    }
    

    public class StateObject
    {
        // Client  socket.
        public Socket WorkSocket;

        // Bytes received so far.
        public int RecvCount;

        // Buffer size.
        public int BufferSize = -1;
        // Buffer.
        public byte[] Buffer;

        // Header buffer size.
        public const int HeaderBufferSize = 4;
        // Header buffer.
        public readonly byte[] HeaderBuffer = new byte[HeaderBufferSize];
    }

    public static class Server
    {
        public static bool AcceptPatches;

        private static bool _running = true;

        public static void Terminate()
        {
            _running = false;
            AllDone.Set();
        }

        // Thread signal.
        private static readonly ManualResetEvent AllDone = new ManualResetEvent(false);

        public static void StartListening()
        {
            // Establish the local endpoint for the socket.
            var localEndPoint = new IPEndPoint(IPAddress.Parse("10.10.20.6"), 11000);

            // Create a TCP/IP socket.
            var listener = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            // Bind the socket to the local endpoint and listen for incoming connections.
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(100);

                while (_running)
                {
                    // Set the event to nonsignaled state.
                    AllDone.Reset();

                    // Start an asynchronous socket to listen for connections.
                    Program.WriteLine("Server is waiting for a connection...");
                    listener.BeginAccept(AcceptCallback, listener);

                    // Wait until a connection is made before continuing.
                    AllDone.WaitOne();
                }

                Program.WriteLine("Server listener closing...");

                listener.Close();

            }
            catch (Exception e)
            {
                Program.WriteLine(e.ToString());
            }
        }

        private static void AcceptCallback(IAsyncResult ar)
        {
            // Signal the main thread to continue.
            AllDone.Set();

            // Get the socket that handles the client request.
            var listener = (Socket)ar.AsyncState;
            var handler = listener.EndAccept(ar);

            Program.WriteLine("Someone connected!!!");

            // Create the state object.
            var state = new StateObject { WorkSocket = handler };

            handler.BeginReceive(state.HeaderBuffer, 0, StateObject.HeaderBufferSize, 0, ReadCallback, state);
        }

        private static void ReadCallback(IAsyncResult ar)
        {
            // Retrieve the state object and the handler socket
            // from the asynchronous state object.
            var state = (StateObject)ar.AsyncState;
            var handler = state.WorkSocket;

            // Read data from the client socket. 
            var bytesRead = handler.EndReceive(ar);

            if (bytesRead <= 0) return;

            if (state.BufferSize == -1)
            {
                if (bytesRead != StateObject.HeaderBufferSize || state.RecvCount != 0) throw new Exception("AGGGRRR");

                var cmd = BitConverter.ToInt32(state.HeaderBuffer, 0);

                if (cmd <= 0) return;

                state.BufferSize = cmd;

                Program.WriteLine("[AGENT] Buffer size is " + state.BufferSize);

                state.Buffer = new byte[state.BufferSize];

                // Not all data received. Get more.
                handler.BeginReceive(state.Buffer, 0, state.BufferSize, 0, ReadCallback, state);

                //if (cmd == -1)
                //{
                //    Program.WriteLine("[AGENT] command newpatch");

                //    if (Directory.Exists("newpatch"))
                //    {
                //        PATCHER.patch("newpatch");
                //        PATCHER.restartall();
                //    }
                //}
            }
            else
            {
                state.RecvCount += bytesRead;

                if (state.RecvCount < state.BufferSize)
                {
                    // Not all data received. Get more.
                    handler.BeginReceive(state.Buffer, state.RecvCount, state.BufferSize - state.RecvCount, 0, ReadCallback, state);
                }
                else if (AcceptPatches)
                {
                    var formatter = new BinaryFormatter();
                    var buffer = new MemoryStream(state.Buffer);
                    var patch = (object[][])formatter.Deserialize(buffer);
                        
                    object[] currentomi;

                    // get a copy of current omi to update it
                    using (var source = File.OpenRead(@"versioning\current\Res-Tex\omi.tex")) currentomi = FormatterOmi.Read(source);

                    DBstuff.UpdateDB(patch, currentomi[0] as object[][]);

                    Patcher.EmptyFolder("temp");
                    Directory.CreateDirectory(@"temp\Res-Tex");

                    using (var dest = File.OpenWrite(@"temp\Res-Tex\omi.tex")) FormatterOmi.Write(dest, currentomi);
                        
                    Patcher.AddNewPatch(new DirectoryInfo("temp"));

                    Patcher.RestartAll();

                    Send(handler, new byte[] { 0xFF, 0xFF, 0xFF, 0xFF });
                }
                else Send(handler, new byte[] { 0xFF, 0xFF, 0, 0 });
            }
        }

        private static void Send(Socket handler, byte[] byteData)
        {
            // Begin sending the data to the remote device.
            handler.BeginSend(byteData, 0, byteData.Length, 0, SendCallback, handler);
        }

        private static void SendCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.
                var handler = (Socket)ar.AsyncState;

                // Complete sending the data to the remote device.
                /*var bytesSent = */ handler.EndSend(ar);

                //WriteLine("Sent {0} bytes to client.", bytesSent);

                handler.Shutdown(SocketShutdown.Both);
                handler.Close();

            }
            catch (Exception e)
            {
                Program.WriteLine(e.ToString());
            }
        }
    }

    

    public static class HttpStuff
    {
        private static HttpListener _listener;

        private static readonly ManualResetEvent Stop = new ManualResetEvent(false);

        // This example requires the System and System.Net namespaces.
        public static void Listen(params string[] prefixes)
        {
            if (!HttpListener.IsSupported)
            {
                Program.WriteLine("Windows XP SP2 or Server 2003 is required to use the HttpListener class.");
                return;
            }

            // URI prefixes are required,
            // for example "http://contoso.com:8080/index/".
            if (prefixes == null || prefixes.Length == 0) throw new ArgumentException("prefixes");

            // Create a listener.
            _listener = new HttpListener();
            // Add the prefixes.
            foreach (var s in prefixes) _listener.Prefixes.Add(s);

            _listener.Start();

            Program.WriteLine("HTPP listening...");

            _listener.BeginGetContext(OnRequest, null);

            Stop.WaitOne();

            _listener.Stop();
        }

        private static void OnRequest(IAsyncResult asyncResult)
        {
            var context = _listener.EndGetContext(asyncResult);

            _listener.BeginGetContext(OnRequest, null);

            var request = context.Request;

            // Obtain a response object.
            var response = context.Response;

            byte[] buffer = null;

            if (request.RawUrl.StartsWith(@"/versioning/patches/"))
            {
                var split = request.RawUrl.Split('_');
                
                if (split.Length == 2)
                {
                    var fname = @"versioning\patches\" + split[1];

                    if (File.Exists(fname)) buffer = File.ReadAllBytes(fname);
                }
            }

            else if (request.RawUrl == @"/versioning/Launcher.atm" ||
                request.RawUrl == @"/versioning/deletefilelist.txt")

                buffer = File.ReadAllBytes("." + request.RawUrl);


            if (buffer == null)
            {
                var ipstr = request.RemoteEndPoint?.Address.ToString() ?? "NULL";

                Program.WriteLine($"[{ipstr}]\tNot found: {request.RawUrl}");

                // Construct a response.
                buffer = System.Text.Encoding.UTF8.GetBytes("");

                response.StatusCode = 404;
            }

            // Get a response stream and write the response to it.
            response.ContentLength64 = buffer.Length;

            using (var output = response.OutputStream)
                try
                {
                    output.Write(buffer, 0, buffer.Length);
                }
                // ReSharper disable once EmptyGeneralCatchClause
                catch (Exception)
                {

                }
        }
    }


    public static class Patcher
    {
        public static void EmptyFolder(string path)
        {
            if (Directory.Exists(path))
            {
                var di = new DirectoryInfo(path);

                foreach (var file in di.GetFiles()) file.Delete();

                foreach (var dir in di.GetDirectories()) dir.Delete(true);
            }
            else Directory.CreateDirectory(path);

        }


        public static string VerToStr(ulong ver)
        {
            return ((ver >> 48) & 0xFFFF) + "." + ((ver >> 32) & 0xFFFF) + "." + ((ver >> 16) & 0xFFFF) + "." + (ver & 0xFFFF);
        }

        public static ulong StrToVer(string str)
        {
            var split = str.Split('.');

            if (split.Length != 4) return 0;

            ushort v0, v1, v2, v3;

            var s0 = ushort.TryParse(split[0], out v0);
            var s1 = ushort.TryParse(split[1], out v1);
            var s2 = ushort.TryParse(split[2], out v2);
            var s3 = ushort.TryParse(split[3], out v3);

            if (!s0 || !s1 || !s2 || !s3) return 0;

            return v3 + ((ulong)v2 << 16) + ((ulong)v1 << 32) + ((ulong)v0 << 48);
        }


        private static readonly Dictionary<ulong, string[]> Changelog;

        private static readonly Dictionary<ulong, string[]> Patchinfo;
        
        private static readonly string[] Deletedfiles;
        
        private static readonly ulong Minversion;

        public static ulong Version;



        // ReSharper disable once UnusedMember.Global
        public static void AddSingleFile(string path, string filename, byte[] data)
        {
            EmptyFolder("temp");

            if (!path.StartsWith(@"\")) path = @"\" + path;

            if (!path.EndsWith(@"\")) path = path + @"\";

            Directory.CreateDirectory("temp" + path);

            File.WriteAllBytes("temp" + path + filename, data);
            File.WriteAllBytes(@"versioning\current" + path + filename, data);

            AddNewPatch(new DirectoryInfo("temp"));
        }

        // ReSharper disable once MemberCanBePrivate.Global
        public static void KillAll()
        {
            foreach (var p in Process.GetProcessesByName("NPCServer"))
            {
                p.Kill();
                p.WaitForExit();
            }
            foreach (var p in Process.GetProcessesByName("FieldServer"))
            {
                p.Kill();
                p.WaitForExit();
            }
            foreach (var p in Process.GetProcessesByName("IMServer"))
            {
                p.Kill();
                p.WaitForExit();
            }
            foreach (var p in Process.GetProcessesByName("LogServer"))
            {
                p.Kill();
                p.WaitForExit();
            }
            foreach (var p in Process.GetProcessesByName("PreServer"))
            {
                p.Kill();
                p.WaitForExit();
            }
        }

        public static void RestartAll()
        {
            new Thread(() =>
            {
                KillAll();

                string[] files = { "PreServer.exe", "LogServer.exe", "IMServer.exe", "FieldServer.exe", "NPCServer.exe" };

                foreach (var file in files)
                {
                    var info = new ProcessStartInfo(file)
                    {
                        UseShellExecute = true,
                        WorkingDirectory = @"..\..\Server\Release\"
                    };
                    
                    Process.Start(info);

                    Thread.Sleep(1000);
                }

            }).Start();
        }


        public static void ChangeVersion(ulong ver)
        {
            Version = ver;

            File.WriteAllBytes(@"versioning\version", BitConverter.GetBytes(Version));

            Program.WriteLine("New version is: " + VerToStr(Version));
        }


        private static void SaveChangeLog()
        {
            // Create a new file stream to write the serialized object to a file
            using (var stream = new FileStream(@"versioning\changelog", FileMode.Create, FileAccess.Write))

                new BinaryFormatter().Serialize(stream, Changelog);
        }

        private static void SavePatchInfo()
        {
            // Create a new file stream to write the serialized object to a file
            using (var stream = new FileStream(@"versioning\patchinfo", FileMode.Create, FileAccess.Write))

                new BinaryFormatter().Serialize(stream, Patchinfo);
        }

        private static void UpdateVersionsCfg()
        {
            var lines = new List<string>();

            for (var v = Minversion; v < Version; v++)
            {
                var newver = Patchinfo.Where(x => x.Key > v).Select(x => x.Key).Min();

                lines.Add("ClientVersion\t" + VerToStr(v) + "\t" + VerToStr(newver));
            }

            if (!lines.Any()) lines.Add("ClientVersion\t" + VerToStr(Version) + "\t" + VerToStr(Version));

            var newlines = lines.Concat(File.ReadAllLines(@"..\..\..\config\versions.cfg").Where(x => !x.StartsWith("ClientVersion")));

            File.WriteAllLines(@"..\..\..\config\versions.cfg", newlines.ToArray());
        }

        private static void GeneratePatches()
        {
            Patchinfo.Clear();

            foreach (var change in Changelog)
            {
                // select the filenames that are not in deletefilelist and are not changed in any newer versions
                var current = change.Value.Where(

                    name => !Deletedfiles.Contains(name) &&
                    !Changelog.Where(x => x.Key > change.Key).Any(x => x.Value.Any(xname => name == xname)));

                var tmp = current.ToArray();

                // if we selected no filenames skip this
                if (tmp.Length == 0) continue;

                // otherwise we will make a patch for this version
                Patchinfo[change.Key] = tmp;
            }

            SavePatchInfo();

            EmptyFolder(@"versioning\patches");

            // make the zip files
            foreach (var patch in Patchinfo)
            {
                using (var stream = new FileStream(@"versioning\patches\" + VerToStr(patch.Key) + ".zip", FileMode.Create, FileAccess.Write))
                using (var archive = new ZipArchive(stream, ZipArchiveMode.Create))
                    foreach (var file in patch.Value.Where(x => !Deletedfiles.Contains(x))) archive.CreateEntryFromFile(@"versioning\current\" + file, file);
            }

            UpdateVersionsCfg();
        }

        public static void AddNewPatch(DirectoryInfo directory, bool force = false)
        {
            if (!directory.Exists) return;
            
            var changes = directory.GetFiles("*.*", SearchOption.AllDirectories).Select(
                // Get a tuple of the relative (to input directory) path and the full contents of each file
                x => new Tuple<string, byte[]>(x.FullName.Substring(directory.FullName.Length + 1), File.ReadAllBytes(x.FullName)));

            if (!force)
            {
                // This lamda determines if this file has not been modified
                Func<Tuple<string, byte[]>, bool> changed = x =>
                {
                    if (!File.Exists(@"versioning\current\" + x.Item1)) return true;

                    var bytes = File.ReadAllBytes(@"versioning\current\" + x.Item1);

                    return !bytes.SequenceEqual(x.Item2);
                };

                // Keep modified files only
                changes = changes.Where(changed);
            }

            var changesCache = changes.ToArray();

            // Return if there are no modified files
            if (changesCache.Length == 0) return;

            var temp = changesCache.Select(x => x.Item1).ToArray();

            // Copy each file into the current version's directory
            foreach (var change in changesCache)
            {
                var filename = @"versioning\current\" + change.Item1;

                var dir = Path.GetDirectoryName(filename);

                if (!Directory.Exists(dir)) Directory.CreateDirectory(dir);

                File.WriteAllBytes(filename, change.Item2);
            }
        
            // Increment version
            ChangeVersion(Version + 1);

            // Add the changes in the changelog
            Changelog[Version] = temp;

            SaveChangeLog();

            GeneratePatches();
        }

        static Patcher()
        {
            Minversion = (ulong)2 << 48;



            if (File.Exists(@"versioning\version"))

                Version = BitConverter.ToUInt64(File.ReadAllBytes(@"versioning\version"), 0);

            else
            {
                Version = Minversion + 1;

                File.WriteAllBytes(@"versioning\version", BitConverter.GetBytes(Version));
            }



            if (File.Exists(@"versioning\changelog"))

                using (var stream = new FileStream(@"versioning\changelog", FileMode.Open, FileAccess.Read))

                    Changelog = (Dictionary<ulong, string[]>)new BinaryFormatter().Deserialize(stream);

            else Changelog = new Dictionary<ulong, string[]>();

            

            if (File.Exists(@"versioning\patchinfo"))

                using (var stream = new FileStream(@"versioning\patchinfo", FileMode.Open, FileAccess.Read))

                    Patchinfo = (Dictionary<ulong, string[]>)new BinaryFormatter().Deserialize(stream);

            else Patchinfo = new Dictionary<ulong, string[]>();



            Deletedfiles = File.Exists(@"versioning\deletefilelist.txt") ?
                
                File.ReadAllLines(@"versioning\deletefilelist.txt") : new string[0];
        }
    }
}