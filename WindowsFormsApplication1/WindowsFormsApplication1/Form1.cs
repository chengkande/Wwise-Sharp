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
        WwiseSharpGameObject WwiseObject2;
        private uint globalObjectID = 100;
        private bool isCallbackEnabled = false;
        private bool cptSaxHasStarted = false;
        private bool cptSaxCanStart = false;
        long timer = 1000/60;
        long time = 0;
        long latency = 0;
        uint musicPlayingID = 0;
        WwiseSharpMusicInfo musicInfo;
        System.Diagnostics.Stopwatch stopwatch = new System.Diagnostics.Stopwatch();
        string path = AppDomain.CurrentDomain.BaseDirectory;
        float targetVolume = 100f;
        float currentVolume = 100f;
        string currentTrack;
        public Form1()
        {
            InitializeComponent();
            Wwise.Init();
            Console.WriteLine(path);
            Wwise.SetBasePath(path+"Wwise/Banks/");
            //Wwise.SetAudioSrcPath("C:/Users/Miles/Documents/GitHub/WwiseSharp/wwisesharp/WindowsFormsApplication1/WindowsFormsApplication1/Wwise/Banks/"); //only for external sources
            //Wwise.InitReverb();
            Wwise.LoadBank("Init.bnk");
            Wwise.LoadBank("global.bnk");
            Wwise.SetLanguage("English(US)");
            Wwise.LoadBank("ambience.bnk");
            Wwise.LoadBank("music.bnk");
            //Wwise.LoadBank("fowl_mouth.bnk");
            //Wwise.LoadBank("captain_sax.bnk");
           
            WwiseObject = Wwise.RegisterGameObject(globalObjectID, "global");
            WwiseObject2 = Wwise.RegisterGameObject((uint)1, "local");
            WwiseObject.PostEvent("Enable_Reverb");

            //set Listener back 50 so we can hear the left/right
            WwiseSharpTransform listenerPos = new WwiseSharpTransform(0,0,-50,0,0,1,0,1,0);

            Wwise.SetListenerPosition(listenerPos);
            stopwatch.Restart();

            Application.Idle += HandleApplicationIdle;
        }
        void HandleApplicationIdle(object sender, EventArgs e)
        {
            while (IsApplicationIdle())
            {
                if (timer <= stopwatch.ElapsedMilliseconds)
                {
                    if (targetVolume > currentVolume)
                    {
                        currentVolume += 0.5f;
                        Wwise.SetGlobalRTPCValue("Music_Volume", currentVolume);
                        stopwatch.Restart();
                    }
                    if (targetVolume < currentVolume)
                    {
                        currentVolume -= 0.5f;
                        Wwise.SetGlobalRTPCValue("Music_Volume", currentVolume);
                        stopwatch.Restart();
                    }
                }
                
                /*
                if(isCallbackEnabled)
                {
                    //do callback stuff here
                    if (timer <= stopwatch.ElapsedMilliseconds)
                    {
                        //if (WwiseObject.GetMarkerStatus()) //Don't need to wait for callback marker if we're setting a timer
                        //{
                            
                            if (!cptSaxHasStarted && cptSaxCanStart)
                            {
                                time = stopwatch.ElapsedMilliseconds;
                                //WwiseObject2.PostEvent("popup_social_rank_up");
                                WwiseObject.PostEvent("captain_sax_full_loop_start");
                                cptSaxHasStarted = true;
                                Console.WriteLine("midi started " + time + " ms after being set for " + timer + " ms");
                            }
                       // }
                    }
                }
                 * */
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
            WwiseObject.PostEvent("ambience_princess_room_play"); 
        }

        private void button2_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent("ambience_princess_room_stop");
        }
        
        private void button3_Click(object sender, EventArgs e)
        {
            //change reverb type to vents
            Wwise.SetBusEffect(3854292571);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //change reverb type to cave
            Wwise.SetBusEffect(4122393694);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            //play footstep on local object
            WwiseObject.PostEvent("movement_footstep");
        }

        private void button6_Click(object sender, EventArgs e)
        {
            //set switch to lvl1
            WwiseObject.SetSwitch("Surface_Types", "Dirt");
        }

        private void button7_Click(object sender, EventArgs e)
        {
            //set switch to tile
            WwiseObject.SetSwitch("Surface_Types", "Room");
        }

        private void button8_Click(object sender, EventArgs e)
        {
            //weapon
            WwiseObject2.PostEvent("fowl_mouth_use");
        }

        private void button9_Click(object sender, EventArgs e)
        {
            //change to - play music sync event
            
            WwiseSharpTransform objPos = new WwiseSharpTransform(-10,0,0,0,0,1,0,1,0);

            WwiseObject2.SetSinglePosition(objPos);
            

            //WwiseObject2.PostMusicSyncEvent_Bar("test_music_01");
        }

        private void button10_Click(object sender, EventArgs e)
        {
            //change to updateRTPC for tempo
            
            WwiseSharpTransform objPos = new WwiseSharpTransform(0,0,0,0,0,1,0,1,0);

            WwiseObject2.SetSinglePosition(objPos);
            
            //WwiseObject2.PostEvent("set_tempo_180");
        }

        private void button11_Click(object sender, EventArgs e)
        {
            //change to Update Game Parameter event
            
            WwiseSharpTransform objPos = new WwiseSharpTransform(10,0,0,0,0,1,0,1,0);

            WwiseObject2.SetSinglePosition(objPos);
            
            //Wwise.SetGlobalRTPCValue("Captain_Sax_Tempo", 64f);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            /*
            WwiseSharpTransform listenerPos = new WwiseSharpTransform();
            listenerPos.Xposition = 0;
            listenerPos.Yposition = 0;
            listenerPos.Zposition = -50;
            listenerPos.XorientationFront = 0;
            listenerPos.YorientationFront = 0;
            listenerPos.ZorientationFront = 1;
            listenerPos.XorientationTop = 0;
            listenerPos.YorientationTop = 1;
            listenerPos.ZorientationTop = 0;

            Wwise.SetListenerPosition(listenerPos);
             * */
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        
        //start midi  -- this currently breaks if being posted during music pre-entry also you need to post a musicsync event or you'll get a dividebyzero
        private void button14_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent("captain_sax_loop_intro");
            musicInfo = WwiseObject.GetPlayingSegmentInfo(musicPlayingID);

            //set a timer to post event 200ms (or whatever latency there may be later) before next bar
            timer = ((long)musicInfo.barDuration - ((long)musicInfo.currentPosition - (long)musicInfo.preEntryDuration) % (long)musicInfo.barDuration - latency) > 0 ? 
            ((long)musicInfo.barDuration - ((long)musicInfo.currentPosition - (long)musicInfo.preEntryDuration) % (long)musicInfo.barDuration - latency) :
            ((long)musicInfo.barDuration - ((long)musicInfo.currentPosition - (long)musicInfo.preEntryDuration) % (long)musicInfo.barDuration - latency) + (long)musicInfo.barDuration;

            stopwatch.Restart();

            cptSaxCanStart = true;
            Console.WriteLine("timer set for " + timer.ToString() + " ms");
           // Console.WriteLine("current position: " + musicInfo.currentPosition);
           // Console.WriteLine("pre-entry duration: " + musicInfo.preEntryDuration);
           // Console.WriteLine("bar duration: " + musicInfo.barDuration);
            //Console.WriteLine("current position in bar: " + ((musicInfo.currentPosition - musicInfo.preEntryDuration) % musicInfo.barDuration));
        }
        //stop music
        private void button15_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent(currentTrack+"_stop");
            isCallbackEnabled = false;
        }
        //stop midi
        private void button17_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent("captain_sax_stop");
            cptSaxCanStart = false;
            cptSaxHasStarted = false;
        }
        //attack
        private void button16_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent("captain_sax_attack_enable");
        }
        //un-attack
        private void button18_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent("captain_sax_attack_disable");
        }

        //LoadBanks
        private void button19_Click(object sender, EventArgs e)
        {
            Wwise.LoadBank("paul.bnk");
        }

        //test banks
        private void button20_Click(object sender, EventArgs e)
        {

            WwiseObject2.PostEvent("paul_greet");
        }

        //prepare Events
        private void button21_Click(object sender, EventArgs e)
        {
            Wwise.SetState("Main_Quest_Progress", "Dandelions");
            
        }
        //play music
        private void button12_Click(object sender, EventArgs e)
        {
            /*
            isCallbackEnabled = true;
            WwiseObject.PostMusicSyncEvent_Bar("furryfuneral_play");
            currentTrack = "furryfuneral";
            musicPlayingID = WwiseObject.syncPlayingID;
            */
            Wwise.SetState("Noir_Intro_Music", "layer_4");
        }
        //play mx 2
        private void button13_Click(object sender, EventArgs e)
        {
            /*
            isCallbackEnabled = true;
            WwiseObject.PostMusicSyncEvent_Bar("doingsciencehigh_play");
            currentTrack = "doingsciencehigh";
            musicPlayingID = WwiseObject.syncPlayingID;
            */
            Wwise.SetState("Noir_Intro_Music", "layer_2");
        }
        // mx3
        private void button22_Click(object sender, EventArgs e)
        {
            /*
            isCallbackEnabled = true;
            WwiseObject.PostMusicSyncEvent_Bar("bloodonthecourt_play");
            currentTrack = "bloodonthecourt";
            musicPlayingID = WwiseObject.syncPlayingID;
            */
            Wwise.SetState("Noir_Intro_Music", "layer_3");
        }
        //mx2
        private void button23_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent("doingsciencelow_play");
            currentTrack = "doingsciencelow";
            
        }
        //mx3
        private void button24_Click(object sender, EventArgs e)
        {
            targetVolume = 50f;
            WwiseObject.PostEvent("doingsciencemed_play");
            WwiseObject.PostEvent(currentTrack+"_stop");
            currentTrack = "doingsciencemed";
            
        }
        //stop
        private void button25_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent(currentTrack+"_stop");
        }
        //pause
        private void button26_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent(currentTrack+"_pause");
        }

        private void button27_Click(object sender, EventArgs e)
        {
            WwiseObject.PostEvent(currentTrack+"_resume");
        }
        //load more banks
        private void button28_Click(object sender, EventArgs e)
        {
            Wwise.SetState("Main_Quest_Progress", "Science");
        }
    }
}
