using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FallingObject : MonoBehaviour {

	public int speed = 10;
	public int points = 5;
	public GameObject player;


	// Use this for initialization
	void Start () {
		player = GameObject.FindGameObjectWithTag( "Player" );
		speed = Random.Range( 10, 20 );
	}
	
	// Update is called once per frame
	void Update () {
		// Move object down the screen
		//transform.Rotate(Vector3.right * Time.deltaTime);
		transform.Translate( Vector3.down * Time.deltaTime * speed );

		// Move object back to the top of the screen
		if( transform.position.y < -19.9f ) {
			MoveToTop();
			player.SendMessage( "ScorePoints", points );
		}		
	}

	void MoveToTop() {
		float rand = Random.Range( -19.2f, 19.2f );

		transform.position = new Vector3( rand, 20.1f, transform.position.z );
		speed = Random.Range( 10, 20 );
	}

	void OnTriggerEnter2D( Collider2D collider ) {
		if( collider.CompareTag( "Player" ) ) {
			player.SendMessage( "LoseLife" );
			MoveToTop();
		}
	}
}