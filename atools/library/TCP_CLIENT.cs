using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace library
{
    class TCP_CLIENT
    {
        public Socket client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);    
        private ManualResetEvent ConnectDone = new ManualResetEvent(false);
        private ManualResetEvent SendDone = new ManualResetEvent(false);
        private ManualResetEvent ReceiveDone = new ManualResetEvent(false);
        public Thread ClientThread = null;


        private void ConnectionCallBack(IAsyncResult ar)
        {
            Socket client = (Socket)ar.AsyncState;
            client.EndConnect(ar);

            //AppendTextBox("[System] Greetings Infidel Are you ready ?");
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
                ErrorLoger.LogErrors(ex);   
            }
        }
        private void StartClient()
        {
            try
            {
                IPHostEntry ipHostInfo = Dns.GetHostEntry("aresonline.in");
                IPAddress ipaddress = ipHostInfo.AddressList[0];
                IPEndPoint RemoteEP = new IPEndPoint(ipaddress, 6667);
                client.BeginConnect(RemoteEP, new AsyncCallback(ConnectionCallBack), client);
                ConnectDone.WaitOne();
              
              //  Send(client, "PRIVMSG " + "#zeyden_x :" + _WelcomeMessage + "\r\n");
                SendDone.WaitOne();
                Receive(client);
                ReceiveDone.WaitOne();
            }
            catch (Exception ex)
            {
                ErrorLoger.LogErrors(ex);
            }

        }
        private void AppendTextBox(string text)
        {
           
            try
            {
                //if (messages.InvokeRequired)
                //{
                //    this.messages.Invoke(new Action<string>(AppendTextBox), new object[] { text });
                //    return;
                //}
                //if (messages.Text.Length > 2048)
                //{
                //    messages.Text = "";
                //}


                //messages.Text += "<" + time + "> " + text + "\r\n";
                //messages.SelectionStart = messages.Text.Length;
                //messages.ScrollToCaret();

            }
            catch { }
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
                ErrorLoger.LogErrors(ex);
            }
        }
   
        public void Initialize()
        {

            if (ClientThread == null)
            { 
                ClientThread = new Thread(StartClient);
                ClientThread.Start();          
            }
            else
            {
               
            }
        }
        public void SendMsg(string message)
        {
            byte[] temp = Encoding.UTF8.GetBytes(message);
            client.BeginSend(temp, 0, temp.Length, 0, new AsyncCallback(SendCallBack), client);
        }
      
    }
    public class StateObject
    {
        public Socket WorkSocket = null;
        public byte[] buffer = new byte[8192];
    }
}
