using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Library
{
    /// <summary>
    /// Review by panoskj:
    /// Refactored the code a bit without changing functionality.
    /// There is no clean way to close this client (and we need this).
    /// We will also not use a hardcoded method to handle received messages;
    /// instead we will set a callback function that receives a byte array (incoming message).
    /// </summary>
    public class ClientTCP
    {
        ManualResetEvent ConnectDone = new ManualResetEvent(false);
        ManualResetEvent SendDone = new ManualResetEvent(false);
        ManualResetEvent ReceiveDone = new ManualResetEvent(false);

        public string ServerAddress { get; }
        public ushort ServerPort { get; }
        /// <summary>
        /// Right not it's not used (todo : needs to be implemented).
        /// </summary>
        public Func<byte[]> RecvCallback { get; }
        public Socket ClientSocket { get; }
        public Thread ClientThread { get; }



        private void StartClient()
        {
            try
            {
                var ipHostInfo = Dns.GetHostEntry(ServerAddress);
                var ipaddress = ipHostInfo.AddressList[0];
                var RemoteEP = new IPEndPoint(ipaddress, ServerPort);

                ClientSocket.BeginConnect(RemoteEP, new AsyncCallback(ConnectionCallBack), ClientSocket);

                ConnectDone.WaitOne();

                SendDone.WaitOne();

                Receive(ClientSocket);

                ReceiveDone.WaitOne();
            }
            catch (Exception ex)
            {
                ErrorLogger.LogException(ex);
            }

        }

        private void ConnectionCallBack(IAsyncResult ar)
        {
            var client = (Socket)ar.AsyncState;
            client.EndConnect(ar);
            
            ConnectDone.Set();
        }


        private void Receive(Socket client)
        {
            StateObject state = new StateObject();
            state.WorkSocket = client;
            client.BeginReceive(state.buffer, 0, state.buffer.Length, 0, new AsyncCallback(ReceiveCallBack), state);
        }
        private void ReceiveCallBack(IAsyncResult ar)
        {
            try
            {
                StateObject state = (StateObject)ar.AsyncState;
                Socket client = state.WorkSocket;
                if (client.ReceiveBufferSize > 0)
                {
                    client.BeginReceive(state.buffer, 0, state.buffer.Length, 0, new AsyncCallback(ReceiveCallBack), state);
                }   
                ReceiveDone.Set();

            }
            catch (Exception ex)
            {
                ErrorLogger.LogException(ex);   
            }
        }




        private void Send(Socket client, string Data)
        {
            byte[] byteData = Encoding.ASCII.GetBytes(Data);
            client.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallBack), client);
        }
        private void SendCallBack(IAsyncResult ar)
        {
            try
            {
                Socket client = (Socket)ar.AsyncState;
                int bytesSend = client.EndSend(ar);
                SendDone.Set();
            }
            catch (Exception ex)
            {
                ErrorLogger.LogException(ex);
            }
        }
   

        public ClientTCP(string Address, ushort Port, Action<byte[]> RecvCallback)
        {
            ServerAddress = Address;
            ServerPort = Port;

            ClientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            ClientThread = new Thread(StartClient);

            ClientThread.Start();          
        }
        
        public void SendMsg(byte[] msg) => ClientSocket.BeginSend(msg, 0, msg.Length, 0, new AsyncCallback(SendCallBack), ClientSocket);
        
      
    }
    public class StateObject
    {
        public Socket WorkSocket = null;
        public byte[] buffer = new byte[8192];
    }
}
