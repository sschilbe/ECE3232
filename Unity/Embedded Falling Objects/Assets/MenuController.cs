using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using WindowsInput;
using WindowsInput.Native;
using UnityEngine.EventSystems;

/**
 *
 * @file
 * MenuController.cs
 *
 * @brief
 * Control the simulated keypresses from the controller and which menu button is selected
 *
 * Author: Meneley, Julia | Schilbe, Seth
 * Date Created: 19/02/2019
 * Last Modified: 27/03/2019
 */
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

		button = GameObject.Find( "BackButton" );

        if( button != null ) {
            EventSystem.current.SetSelectedGameObject( button );
            return;
        }

        button = GameObject.Find( "PlayAgainButton" );

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
            } else if( controller.ySpeed > 0.5f || controller.ySpeed < -0.5f ) {
                input.Keyboard.KeyPress( VirtualKeyCode.RETURN );
            }
        }

        return changed;
    }
}
