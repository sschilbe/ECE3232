using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using WindowsInput;
using WindowsInput.Native;
using UnityEngine.EventSystems;

public class MenuController : MonoBehaviour {
    public static Controller controller;

    private static GameObject selected;
    private static InputSimulator input = new InputSimulator();

    public void MenuChanged() {
		GameObject button = GameObject.Find( "ControllersButton" );
        
        if( button != null ) {
            EventSystem.current.SetSelectedGameObject( button );
            return;
        }

        /* button = GameObject.Find( "SettingsButton" );
        
        if( button != null ) {
            EventSystem.current.SetSelectedGameObject( button );
            return;
        } */

		button = GameObject.Find( "BackButton" );

        if( button != null ) {
            EventSystem.current.SetSelectedGameObject( button );
            return;
        }
    }

    public static bool changeSelection() {
        bool changed = false;
        
        if( controller != null ) {
            if( controller.xSpeed > 0.5f ) {
                input.Keyboard.KeyPress( VirtualKeyCode.DOWN );
                changed = true;
            } else if( controller.xSpeed < -0.5f ) {
                input.Keyboard.KeyPress( VirtualKeyCode.UP );
                changed = true;
            }
        }

        return changed;
    }
}
