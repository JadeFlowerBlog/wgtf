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

namespace NGTTestAutomation.Generic_app_UI
{
#pragma warning disable 0436 //(CS0436) The type 'type' in 'assembly' conflicts with the imported type 'type2' in 'assembly'. Using the type defined in 'assembly'.
    /// <summary>
    ///The Layou_undock_redock recording.
    /// </summary>
    [TestModule("1c4cbc78-05ca-44cb-a7fa-cdf5dc198d3a", ModuleType.Recording, 1)]
    public partial class Layou_undock_redock : ITestModule
    {
        /// <summary>
        /// Holds an instance of the NGTTestAutomation.NGTTestAutomationRepository repository.
        /// </summary>
        public static NGTTestAutomation.NGTTestAutomationRepository repo = NGTTestAutomation.NGTTestAutomationRepository.Instance;

        static Layou_undock_redock instance = new Layou_undock_redock();

        /// <summary>
        /// Constructs a new instance.
        /// </summary>
        public Layou_undock_redock()
        {
        }

        /// <summary>
        /// Gets a static instance of this recording.
        /// </summary>
        public static Layou_undock_redock Instance
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

        }

#region Image Feature Data
#endregion
    }
#pragma warning restore 0436
}
