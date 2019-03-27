using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.EventSystems;

/**
 *
 * @file
 * MainMenu.cs
 *
 * @brief
 * Initialization for main menu
 *
 * Author: Meneley, Julia | Schilbe, Seth
 * Date Created: 19/02/2019
 * Last Modified: 27/03/2019
 */
public class MainMenu : MonoBehaviour {
	public void Start() {
		GameObject button = GameObject.Find( "ControllersButton" );
        
        if( button != null ) {
            EventSystem.current.SetSelectedGameObject( button );
            return;
        }
	}

	public void PlayGame() {
		SceneManager.LoadScene( SceneManager.GetActiveScene().buildIndex + 1 );
	}

	public void QuitGame() {
		Application.Quit();
	}
}
