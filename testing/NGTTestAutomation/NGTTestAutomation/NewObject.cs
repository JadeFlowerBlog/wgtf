﻿///////////////////////////////////////////////////////////////////////////////
//
// This file was automatically generated by RANOREX.
// DO NOT MODIFY THIS FILE! It is regenerated by the designer.
// All your modifications will be lost!
// http://www.ranorex.com
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Drawing;
using System.Threading;
using WinForms = System.Windows.Forms;

using Ranorex;
using Ranorex.Core;
using Ranorex.Core.Testing;
using Ranorex.Core.Repository;

namespace NGTTestAutomation
{
#pragma warning disable 0436 //(CS0436) The type 'type' in 'assembly' conflicts with the imported type 'type2' in 'assembly'. Using the type defined in 'assembly'.
    /// <summary>
    ///The NewObject recording.
    /// </summary>
    [TestModule("dbe4d3f5-b07d-45aa-ac93-8ae098a5b09d", ModuleType.Recording, 1)]
    public partial class NewObject : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomationRepository repo = NGTTestAutomationRepository.Instance;

        static NewObject instance = new NewObject();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public NewObject()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static NewObject Instance
        {
            get { return instance; }
        }

#region Variables

#endregion

        /// <summary>
        /// Starts the replay of the static recording <see cref="Instance"/>.
        /// </summary>
        [System.CodeDom.Compiler.GeneratedCode("Ranorex", "5.4.5")]
        public static void Start()
        {
            TestModuleRunner.Run(Instance);
        }

        /// <summary>
        /// Performs the playback of actions in this recording.
        /// </summary>
        /// <remarks>You should not call this method directly, instead pass the module
        /// instance to the <see cref="TestModuleRunner.Run(ITestModule)"/> method
        /// that will in turn invoke this method.</remarks>
        [System.CodeDom.Compiler.GeneratedCode("Ranorex", "5.4.5")]
        void ITestModule.Run()
        {
            Mouse.DefaultMoveTime = 300;
            Keyboard.DefaultKeyPressTime = 100;
            Delay.SpeedFactor = 1.0;

            Init();

            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'MainWindow.Column1' at 80;161.", repo.MainWindow.Column1Info, new RecordItemIndex(0));
            repo.MainWindow.Column1.Click("80;161");
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'MainWindow.Column1' at 80;161.", repo.MainWindow.Column1Info, new RecordItemIndex(1));
            repo.MainWindow.Column1.Click("80;161");
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'MainWindow.Column1' at 80;161.", repo.MainWindow.Column1Info, new RecordItemIndex(2));
            repo.MainWindow.Column1.Click("80;161");
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'MainWindow.Column1' at 80;161.", repo.MainWindow.Column1Info, new RecordItemIndex(3));
            repo.MainWindow.Column1.Click("80;161");
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'MainWindow.Column1' at 80;161.", repo.MainWindow.Column1Info, new RecordItemIndex(4));
            repo.MainWindow.Column1.Click("80;161");
            Delay.Milliseconds(200);
            
        }

#region Image Feature Data
#endregion
    }
#pragma warning restore 0436
}
