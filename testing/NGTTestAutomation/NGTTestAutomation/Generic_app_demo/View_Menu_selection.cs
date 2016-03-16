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
    ///The View_Menu_selection recording.
    /// </summary>
    [TestModule("0057bfca-3540-48d9-ba3c-bbfd887e62df", ModuleType.Recording, 1)]
    public partial class View_Menu_selection : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomation.NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomation.NGTTestAutomationRepository repo = NGTTestAutomation.NGTTestAutomationRepository.Instance;

        static View_Menu_selection instance = new View_Menu_selection();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public View_Menu_selection()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static View_Menu_selection Instance
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

            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'Generic_app.MainWindow.View' at 4;4.", repo.Generic_app.MainWindow.ViewInfo, new RecordItemIndex(0));
            repo.Generic_app.MainWindow.View.Click(new Location(View_Screenshot1, "4;4", View_Screenshot1_Options));
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'View.Macros' at 5;4.", repo.View.MacrosInfo, new RecordItemIndex(1));
            repo.View.Macros.Click(new Location(Macros_Screenshot1, "5;4", Macros_Screenshot1_Options));
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Mouse", "Mouse Left Click item 'Generic_app.MainWindow.View' at 4;4.", repo.Generic_app.MainWindow.ViewInfo, new RecordItemIndex(2));
            repo.Generic_app.MainWindow.View.Click(new Location(View_Screenshot1_2, "4;4", View_Screenshot1_2_Options));
            Delay.Milliseconds(200);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.Macros'.", repo.View.MacrosInfo, new RecordItemIndex(3));
            Validate.ContainsImage(repo.View.MacrosInfo, Macros_Screenshot1_2, Macros_Screenshot1_2_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.History'.", repo.View.HistoryInfo, new RecordItemIndex(4));
            Validate.ContainsImage(repo.View.HistoryInfo, History_Screenshot1, History_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.TreeExpansionTest'.", repo.View.TreeExpansionTestInfo, new RecordItemIndex(5));
            Validate.ContainsImage(repo.View.TreeExpansionTestInfo, TreeExpansionTest_Screenshot1, TreeExpansionTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.CustomModelInterface'.", repo.View.CustomModelInterfaceInfo, new RecordItemIndex(6));
            Validate.ContainsImage(repo.View.CustomModelInterfaceInfo, CustomModelInterface_Screenshot1, CustomModelInterface_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.ActiveFiltersControlTest'.", repo.View.ActiveFiltersControlTestInfo, new RecordItemIndex(7));
            Validate.ContainsImage(repo.View.ActiveFiltersControlTestInfo, ActiveFiltersControlTest_Screenshot1, ActiveFiltersControlTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.ListModelTest'.", repo.View.ListModelTestInfo, new RecordItemIndex(8));
            Validate.ContainsImage(repo.View.ListModelTestInfo, ListModelTest_Screenshot1, ListModelTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.ListFilterTest'.", repo.View.ListFilterTestInfo, new RecordItemIndex(9));
            Validate.ContainsImage(repo.View.ListFilterTestInfo, ListFilterTest_Screenshot1, ListFilterTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.TreeModelTest'.", repo.View.TreeModelTestInfo, new RecordItemIndex(10));
            Validate.ContainsImage(repo.View.TreeModelTestInfo, TreeModelTest_Screenshot1, TreeModelTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.WGContextMenuTest'.", repo.View.WGContextMenuTestInfo, new RecordItemIndex(11));
            Validate.ContainsImage(repo.View.WGContextMenuTestInfo, WGContextMenuTest_Screenshot1, WGContextMenuTest_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.PythonTest1'.", repo.View.PythonTest1Info, new RecordItemIndex(12));
            Validate.ContainsImage(repo.View.PythonTest1Info, PythonTest1_Screenshot1, PythonTest1_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.PythonTest2'.", repo.View.PythonTest2Info, new RecordItemIndex(13));
            Validate.ContainsImage(repo.View.PythonTest2Info, PythonTest2_Screenshot1, PythonTest2_Screenshot1_Options);
            Delay.Milliseconds(0);
            
            Report.Log(ReportLevel.Info, "Validation", "Validating ContainsImage (Screenshot: 'Screenshot1' with region {X=0,Y=0,Width=196,Height=23}) on item 'View.PrototypeCustomPanel'.", repo.View.PrototypeCustomPanelInfo, new RecordItemIndex(14));
            Validate.ContainsImage(repo.View.PrototypeCustomPanelInfo, PrototypeCustomPanel_Screenshot1, PrototypeCustomPanel_Screenshot1_Options);
            Delay.Milliseconds(0);
            
        }

#region Image Feature Data
        CompressedImage View_Screenshot1
        { get { return repo.Generic_app.MainWindow.ViewInfo.GetScreenshot1(new Rectangle(15, 6, 14, 8)); } }

        Imaging.FindOptions View_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage Macros_Screenshot1
        { get { return repo.View.MacrosInfo.GetScreenshot1(new Rectangle(28, 4, 9, 12)); } }

        Imaging.FindOptions Macros_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage View_Screenshot1_2
        { get { return repo.Generic_app.MainWindow.ViewInfo.GetScreenshot1(new Rectangle(15, 6, 14, 8)); } }

        Imaging.FindOptions View_Screenshot1_2_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage Macros_Screenshot1_2
        { get { return repo.View.MacrosInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions Macros_Screenshot1_2_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage History_Screenshot1
        { get { return repo.View.HistoryInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions History_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage TreeExpansionTest_Screenshot1
        { get { return repo.View.TreeExpansionTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions TreeExpansionTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage CustomModelInterface_Screenshot1
        { get { return repo.View.CustomModelInterfaceInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions CustomModelInterface_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage ActiveFiltersControlTest_Screenshot1
        { get { return repo.View.ActiveFiltersControlTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions ActiveFiltersControlTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage ListModelTest_Screenshot1
        { get { return repo.View.ListModelTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions ListModelTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage ListFilterTest_Screenshot1
        { get { return repo.View.ListFilterTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions ListFilterTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage TreeModelTest_Screenshot1
        { get { return repo.View.TreeModelTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions TreeModelTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage WGContextMenuTest_Screenshot1
        { get { return repo.View.WGContextMenuTestInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions WGContextMenuTest_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage PythonTest1_Screenshot1
        { get { return repo.View.PythonTest1Info.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions PythonTest1_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage PythonTest2_Screenshot1
        { get { return repo.View.PythonTest2Info.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions PythonTest2_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

        CompressedImage PrototypeCustomPanel_Screenshot1
        { get { return repo.View.PrototypeCustomPanelInfo.GetScreenshot1(new Rectangle(0, 0, 196, 23)); } }

        Imaging.FindOptions PrototypeCustomPanel_Screenshot1_Options
        { get { return Imaging.FindOptions.Default; } }

#endregion
    }
#pragma warning restore 0436
}
