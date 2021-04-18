package com.tolkiana.spotifyplayer

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.View
import android.widget.TextView
import kotlinx.android.synthetic.main.activity_player.*
import kotlinx.coroutines.*
import java.util.*
import kotlin.math.abs


class PlayerActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        val playlistDict=intent.getStringExtra("playlistDict")
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_player)
        setImage()
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
        setImage()

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

    private fun setText(text: TextView, value: String) {
        runOnUiThread(
            Runnable {
                Log.d("textset", "runOnUiThread")
                runOnUiThread { text.text = value }
            })
    }

    private fun setImage() {
        runOnUiThread(
                Runnable {
                    Log.d("imgset", "runOnUiThread")
                    runOnUiThread {
                        SpotifyService.getCurrentTrackImage {
                            trackImageView.setImageBitmap(it)
                        }
                    }
                })
    }

    fun rhythm(dict: Map<String, Song>) {
        var waitTime = firstSong(dict)
        CoroutineScope(Dispatchers.IO).launch {
            delay(2500)
            setText(actualBattery, "100 %")
            withContext(Dispatchers.Main) {
                setImage()
            }
        }
        CoroutineScope(Dispatchers.IO).launch {
            delay(5000)
            var status = true
            withContext(Dispatchers.IO) {
                while(true) {
                    if (status){
                        status = false
                        GlobalScope.launch {
                        delay(waitTime.toLong() - 30000)
                        waitTime = queueNew(dict)
                        GlobalScope.launch {
                            status = true
                            delay(32000)
                            setImage()
                        }
                    }
                    }
                }
            }
        }
    }

    private fun firstSong(dict: Map<String, Song>): Int {
        val random = Random()
        var first = dict.entries.elementAt(random.nextInt(dict.size))
        Log.d("a", first.toString())
        SpotifyService.play(first.key)
        setText(actualTempo, first.value.tempo.toString())
        return first.value.duration
    }

    fun queueNew(dict: Map<String, Song>): Int {
        var waitTime = 0
        val random = Random()
        var tempo = random.nextGaussian() * 35 + 150
        Log.d("TEMPO", tempo.toString())
        var closestsong = ""
        var closestvalue = 1000.0
        var songtempo = 0.0
        for ((k, v) in dict) {
            if (abs(tempo - v.tempo) < closestvalue) {
                songtempo = v.tempo
                closestvalue = abs(tempo - v.tempo)
                closestsong = k
                waitTime = v.duration
            }
        }
        setText(actualWorkout, tempo.toString())
        setText(actualTempo, songtempo.toString())
        Log.d("SONG", dict[closestsong].toString())
        SpotifyService.queue(closestsong)
        return waitTime
    }
}
