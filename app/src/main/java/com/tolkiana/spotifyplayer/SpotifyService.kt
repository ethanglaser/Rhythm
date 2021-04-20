package com.tolkiana.spotifyplayer

import android.content.Context
import android.graphics.Bitmap
import android.util.Log
import com.fasterxml.jackson.module.kotlin.*
import com.google.gson.GsonBuilder
import com.google.gson.reflect.TypeToken
import com.spotify.android.appremote.api.ConnectionParams
import com.spotify.android.appremote.api.Connector
import com.spotify.android.appremote.api.SpotifyAppRemote
import com.spotify.protocol.types.ImageUri
import com.spotify.protocol.types.Track
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.lang.reflect.Type
import java.net.URL
import java.net.URLEncoder
import java.util.*
import kotlin.math.abs
import kotlinx.android.synthetic.main.activity_player.*
import android.view.View
import android.support.v7.app.AppCompatActivity
import android.os.Bundle

enum class PlayingState {
    PAUSED, PLAYING, STOPPED
}

object SpotifyService {
    private const val CLIENT_ID = "2123317eddd444ffb5f46a2e00417772"
    private const val REDIRECT_URI = "http://localhost:3000/callback/"

    private var mSpotifyAppRemote: SpotifyAppRemote? = null
    private var connectionParams: ConnectionParams = ConnectionParams.Builder(CLIENT_ID)
        .setRedirectUri(REDIRECT_URI)
        .showAuthView(true)
        .build()

    fun connect(context: Context, handler: (connected: Boolean) -> Unit) {

        if (mSpotifyAppRemote?.isConnected == true) {
            handler(true)
            return
        }

        val connectionListener = object : Connector.ConnectionListener {
            override fun onConnected(spotifyAppRemote: SpotifyAppRemote) {
                mSpotifyAppRemote = spotifyAppRemote
                handler(true)
            }

            override fun onFailure(throwable: Throwable) {
                Log.e("SpotifyService", throwable.message, throwable)
                handler(false)
            }
        }
        SpotifyAppRemote.connect(context, connectionParams, connectionListener)
    }

    fun disconnect() {
        SpotifyAppRemote.disconnect(mSpotifyAppRemote)
    }

    fun play(uri: String) {
        mSpotifyAppRemote?.playerApi?.play("spotify:track:" + uri)
    }

    fun queue(uri: String) {
        val finaluri = "spotify:track:" + uri
        mSpotifyAppRemote?.playerApi?.queue((finaluri))
    }

    fun resume() {
        mSpotifyAppRemote?.playerApi?.resume()
    }

    fun pause() {
        mSpotifyAppRemote?.playerApi?.pause()
    }

    fun getCurrentTrack(handler: (track: Track) -> Unit) {
        mSpotifyAppRemote?.playerApi?.playerState?.setResultCallback { result ->
            handler(result.track)
        }
    }

    fun getImage(imageUri: ImageUri, handler: (Bitmap) -> Unit)  {
        mSpotifyAppRemote?.imagesApi?.getImage(imageUri)?.setResultCallback {
            handler(it)
        }
    }

    fun suscribeToChanges() {
        /*mSpotifyAppRemote?.playerApi?.subscribeToPlayerState()?.setEventCallback {
            handler(it.track)
        }*/
        mSpotifyAppRemote?.playerApi?.subscribeToPlayerContext()?.setEventCallback {
            //Log.d("Playlist ID:", it.uri)
        }
    }

    fun getCurrentTrackImage(handler: (Bitmap) -> Unit)  {
        getCurrentTrack {
            Log.d("current", it.toString())
            getImage(it.imageUri) {
                handler(it)
                //trackImageView.setImageBitmap(it)
            }
        }
    }

    fun playingState(handler: (PlayingState) -> Unit) {
        mSpotifyAppRemote?.playerApi?.playerState?.setResultCallback { result ->
            if (result.track.uri == null) {
                handler(PlayingState.STOPPED)
            } else if (result.isPaused) {
                handler(PlayingState.PAUSED)
            } else {
                handler(PlayingState.PLAYING)
            }
        }
    }

    fun jsonHandler(playlistDict: String): Map<String, Song> {
        val gson = GsonBuilder().create()
        val type: Type = object : TypeToken<Map<String?, Song?>?>() {}.type
        val dict: Map<String, Song> = gson.fromJson(playlistDict, type)
        Log.d("playlist", dict.toString())
        return dict
    }

}