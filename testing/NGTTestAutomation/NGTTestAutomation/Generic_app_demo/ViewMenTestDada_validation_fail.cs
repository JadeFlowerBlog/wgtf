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
    ///The ViewMenTestDada_validation_fail recording.
    /// </summary>
    [TestModule("ec8f7523-c091-4e50-9425-b9b13f6d787e", ModuleType.Recording, 1)]
    public partial class ViewMenTestDada_validation_fail : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomation.NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomation.NGTTestAutomationRepository repo = NGTTestAutomation.NGTTestAutomationRepository.Instance;

        static ViewMenTestDada_validation_fail instance = new ViewMenTestDada_validation_fail();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public ViewMenTestDada_validation_fail()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static ViewMenTestDada_validation_fail Instance
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

            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'Generic_app.MainWindow.View' at 5;4.", repo.Generic_app.MainWindow.ViewInfo, new RecordItemIndex(0));
            repo.Generic_app.MainWindow.View.Click(new Location(View_Screenshot1, "5;4", View_Screenshot1_Options));
            Delay.Milliseconds(200);
            
            try {
                Report.Log(ReportLevel.Info, "Validation", "(Optional Action)\r\nValidating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.PropertyTreeTest'.", repo.View.PropertyTreeTestInfo, new RecordItemIndex(1));
                Validate.ContainsImage(repo.View.PropertyTreeTestInfo, PropertyTreeTest_Screenshot1, PropertyTreeTest_Screenshot1_Options, Validate.DefaultMessage, new Validate.Options(){ReportLevelOnFailure=ReportLevel.Success, ReportLevelOnSuccess=ReportLevel.Failure});
                Delay.Milliseconds(0);
            } catch(Exception ex) { Report.Log(ReportLevel.Warn, "Module", "(Optional Action) " + ex.Message, new RecordItemIndex(1)); }
            
        }

#region Image Feature Data
        CompressedImage View_Screenshot1
        { get { return repo.Generic_app.MainWindow.ViewInfo.GetScreenshot1(new Rectangle(12, 4, 17, 10)); } }

        Imaging.FindOptions View_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage PropertyTreeTest_Screenshot1
        { get { return repo.View.PropertyTreeTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions PropertyTreeTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Parse("1;EdgesSobel;0,0,196,23;True;10000000;0ms"); } }

#endregion
    }
#pragma warning restore 0436
}
