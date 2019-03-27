using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class PlayerManager : MonoBehaviour {

	public int speed;
	public float score;
	public Text livesText;
	public Text scoreText;
	public GameObject gameOverPanel;
	public Controller playerController;
	public bool pause = false;
	private float timeToLifeLost = 0;
	// Use this for initialization
	void Start () {
		pause = false;
		speed = 25;	
		score = 0;
		scoreText.text = "Score: " + score.ToString();

		foreach( Controller controller in ControllerList.controllerList ) {
			if( controller.connected && !controller.assigned ) {
				playerController = controller;
				controller.assigned = true;
				controller.sendMessage( controller.GetGameMessage( MsgId.GAME_START_EVENT, false ) );
				break;
			}
		}

		playerController.lives = 3;
		livesText.text = "Lives: " + playerController.lives.ToString();
		
	}
	
	// Update is called once per frame
	void Update () {
		if( timeToLifeLost > 0) {
			if( playerController.lives >= 0 ) {
				playerController.sendMessage( playerController.GetGameMessage( MsgId.PLAYER_HIT_EVENT, true ) );
			}

			timeToLifeLost -= Time.deltaTime;
		}

		// Move the player accordingly
		transform.Translate( Vector3.right * playerController.ySpeed * Time.deltaTime * speed);
	
		// Stop player from leaving left edge
		if( transform.position.x < -19.2 ) {
			transform.position = new Vector3( -19.2f, transform.position.y, transform.position.z );
		}		

		// Stop player from leaving right edge
		if( transform.position.x > 19.2 ) {
			transform.position = new Vector3( 19.2f, transform.position.y, transform.position.z );
		}		
	}

	void ScorePoints( int pointsToAdd ) {
		if( !pause ) {
			score += pointsToAdd;
			scoreText.text = "Score: " + score.ToString();
		}
	}

	void LoseLife() {
		if( timeToLifeLost <= 0.0f ) {
			timeToLifeLost = 0.5f;
			playerController.lives--;
		
			if( playerController.lives >= 0 ) {
				playerController.sendMessage( playerController.GetGameMessage( MsgId.PLAYER_HIT_EVENT, true ) );
				if( playerController.lives == 0 ) {
					GameOver();
				}
			}

			if( playerController.lives < 0 ) {
				livesText.text = "Lives: 0";
			} else {
				livesText.text = "Lives: " + playerController.lives.ToString();
			}
		}	
	}

	void GameOver() {
		//gameObject.SetActive( false );
		pause = true;
		gameOverPanel.SetActive( true );
	}

	public void PlayAgain() {
		playerController.assigned = false;
		SceneManager.LoadScene( SceneManager.GetActiveScene().name );
	}

	public void QuitGame() {
		playerController.assigned = false;
		SceneManager.LoadScene( SceneManager.GetActiveScene().buildIndex - 1 );
	}
}
