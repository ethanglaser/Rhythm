package com.tolkiana.spotifyplayer

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import kotlinx.android.synthetic.main.activity_player.*

class PlayerActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        val playlistDict=intent.getStringExtra("playlistDict")
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_player)
        setupViews()
        setupListeners()
        SpotifyService.jsonHandler(playlistDict)
        //SpotifyService.suscribeToChanges()
    }

    override fun onStop() {
        super.onStop()
        SpotifyService.disconnect()
    }

    private fun setupViews () {
        SpotifyService.getCurrentTrackImage {
            trackImageView.setImageBitmap(it)
        }

        SpotifyService.playingState {
            when(it) {
                PlayingState.PLAYING -> showPauseButton()
                PlayingState.STOPPED -> showPlayButton()
                PlayingState.PAUSED -> showResumeButton()
            }
        }
    }

    private fun setupListeners() {
        playButton.setOnClickListener {
            SpotifyService.play("spotify:album:5L8VJO457GXReKVVfRhzyM")
            showPauseButton()
        }

        pauseButton.setOnClickListener {
            SpotifyService.pause()
            showResumeButton()
        }

        resumeButton.setOnClickListener {
            SpotifyService.resume()
            showPauseButton()
        }

        SpotifyService.suscribeToChanges()
    }

    private fun showPlayButton() {
        playButton.visibility = View.VISIBLE
        pauseButton.visibility = View.GONE
        resumeButton.visibility = View.GONE
    }

    private fun showPauseButton() {
        playButton.visibility = View.GONE
        pauseButton.visibility = View.VISIBLE
        resumeButton.visibility = View.GONE
    }

    private fun showResumeButton() {
        playButton.visibility = View.GONE
        pauseButton.visibility = View.GONE
        resumeButton.visibility = View.VISIBLE
    }
}
