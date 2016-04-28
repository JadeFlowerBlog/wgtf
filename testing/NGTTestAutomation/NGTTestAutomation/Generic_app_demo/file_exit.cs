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

namespace NGTTestAutomation.Generic_app_demo
{
#pragma warning disable 0436 //(CS0436) The type 'type' in 'assembly' conflicts with the imported type 'type2' in 'assembly'. Using the type defined in 'assembly'.
    /// <summary>
    ///The file_exit recording.
    /// </summary>
    [TestModule("959610e8-dc35-4fde-93de-1e632eead6f5", ModuleType.Recording, 1)]
    public partial class File_exit : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomation.NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomation.NGTTestAutomationRepository repo = NGTTestAutomation.NGTTestAutomationRepository.Instance;

        static File_exit instance = new File_exit();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public File_exit()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static File_exit Instance
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

            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'Generic_app.MainWindow.File1' at 12;12.", repo.Generic_app.MainWindow.File1Info, new RecordItemIndex(0));
            repo.Generic_app.MainWindow.File1.Click("12;12");
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'Menu.File.Exit' at 55;15.", repo.Menu.File.ExitInfo, new RecordItemIndex(1));
            repo.Menu.File.Exit.Click("55;15");
            Delay.Milliseconds(200);
            
        }

#region Image Feature Data
#endregion
    }
#pragma warning restore 0436
}
