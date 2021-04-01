package com.tolkiana.spotifyplayer

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import kotlinx.android.synthetic.main.activity_player.*
import kotlinx.coroutines.*
import java.util.*
import kotlin.math.abs
import android.os.Handler;


class PlayerActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        val playlistDict=intent.getStringExtra("playlistDict")
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_player)
        setupViews()
        setupListeners()
        //SpotifyService.suscribeToChanges()
        val dict: Map<String, Song> = SpotifyService.jsonHandler(playlistDict)
        rhythm(dict)
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


    fun rhythm(dict: Map<String, Song>) {
        var waitTime = firstSong(dict)
        CoroutineScope(Dispatchers.IO).launch {
            delay(2500)
            withContext(Dispatchers.Main) {
                SpotifyService.getCurrentTrackImage {
                    trackImageView.setImageBitmap(it)
                }
            }
        }
        CoroutineScope(Dispatchers.IO).launch {
            delay(5000)
            withContext(Dispatchers.Main) {
                while(true) {
                    Thread.sleep(waitTime.toLong() - 30000)
                    waitTime = queueNew(dict)

                    Thread.sleep(31000)
                    SpotifyService.getCurrentTrackImage {
                        trackImageView.setImageBitmap(it)
                    }
                }
                /*while (true) {
                    rhythm(dict)
                    CoroutineScope(Dispatchers.IO).launch {
                        delay(30000)
                        withContext(Dispatchers.Main) {
                            SpotifyService.getCurrentTrackImage {
                                trackImageView.setImageBitmap(it)
                            }
                        }
                    }
                }*/
            }
        }


                    /*SpotifyService.getCurrentTrackImage {
            trackImageView.setImageBitmap(it)
        }*/
    }

    private fun firstSong(dict: Map<String, Song>): Int {
        val random = Random()
        var first = dict.entries.elementAt(random.nextInt(dict.size))
        Log.d("a", first.toString())
        SpotifyService.play(first.key)
        return first.value.duration - 30000
    }

    fun queueNew(dict: Map<String, Song>): Int {
        var waitTime = 0
        val random = Random()
        var tempo = random.nextGaussian() * 35 + 150
        Log.d("TEMPO", tempo.toString())
        var closestsong = ""
        var closestvalue = 100.0
        for ((k, v) in dict) {
            if (abs(tempo - v.tempo) < closestvalue) {
                closestvalue = abs(tempo - v.tempo)
                closestsong = k
                waitTime = v.duration
            }
        }
        Log.d("SONG", dict[closestsong].toString())
        SpotifyService.queue(closestsong)
        return waitTime
    }
}
