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
    ///The SimpleCasesSlideUp recording.
    /// </summary>
    [TestModule("fe4210aa-3276-462b-93b5-2ae968fccb62", ModuleType.Recording, 1)]
    public partial class SimpleCasesSlideUp : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomationRepository repo = NGTTestAutomationRepository.Instance;

        static SimpleCasesSlideUp instance = new SimpleCasesSlideUp();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public SimpleCasesSlideUp()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static SimpleCasesSlideUp Instance
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

            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'MainWindow.WGButtonFrame' at Center.", repo.MainWindow.WGButtonFrameInfo, new RecordItemIndex(0));
            repo.MainWindow.WGButtonFrame.Click();
            Delay.Milliseconds(200);
            
        }

#region Image Feature Data
#endregion
    }
#pragma warning restore 0436
}
