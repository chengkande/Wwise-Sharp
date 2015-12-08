using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using WwiseSharp;
namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        WwiseSharpEngine Wwise = new WwiseSharpEngine();
        WwiseSharpGameObject WwiseObject;
        private uint globalObjectID = 100;
        public Form1()
        {
            InitializeComponent();
            Wwise.Init();
            Wwise.SetBasePath("C:/Users/Miles/Documents/Visual Studio 2013/Projects/WindowsFormsApplication1/WindowsFormsApplication1/Wwise/Banks/");
            Wwise.LoadBank("Init.bnk");
            Wwise.LoadBank("Main.bnk");
            WwiseObject = Wwise.RegisterGameObject(globalObjectID, "global");
            Application.Idle += HandleApplicationIdle;
        }
        void HandleApplicationIdle(object sender, EventArgs e)
        {
            while (IsApplicationIdle())
            {
                Wwise.Update();
            }
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct NativeMessage
        {
            public IntPtr Handle;
            public uint Message;
            public IntPtr WParameter;
            public IntPtr LParameter;
            public uint Time;
            public Point Location;
        }
        [DllImport("user32.dll")]
        public static extern int PeekMessage(out NativeMessage message, IntPtr window, uint filterMin, uint FilterMax, uint remove);

        bool IsApplicationIdle()
        {
            NativeMessage result;
            return PeekMessage(out result, IntPtr.Zero, (uint)0, (uint)0, (uint)0) == 0;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent("ItemSpawn");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent("ItemAmmo");
        }
    }
}
