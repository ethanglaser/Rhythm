import requests
import json
import sys
import os
import time
from pprint import pprint
from matplotlib import pyplot as plt
import spotipy.util as util
import numpy as np


def getHeaders(oAuth):
    headers = {
    'Accept': 'application/json',
    'Content-Type': 'application/json',
    'Authorization': 'Bearer ' + oAuth,
    }
    return headers

def getToken(username, clientID, clientSecret, redirectURI, scopes):
    token = util.prompt_for_user_token(username=username, scope=scopes, client_id=clientID, client_secret=clientSecret, redirect_uri=redirectURI)
    return token 

def getPlaylistID(headers):
    response = requests.get(url='https://api.spotify.com/v1/me/player', headers=headers)
    playback = json.loads(response.content)['context']['uri'].split(':')
    if 'playlist' in playback:
        playlistID = playback[-1]
    return playlistID

def getCurrentProgress(headers):
    response = requests.get(url='https://api.spotify.com/v1/me/player', headers=headers)
    playing = json.loads(response.content)
    return playing["item"]["duration_ms"] - playing["progress_ms"]

def getPlaylistInfo(headers, playlistID):
    songIDs = set()
    tempos = []
    songdict = {}
    while len(songIDs) % 100 == 0:
        currentIDs = []
        response2 = requests.get(url='https://api.spotify.com/v1/playlists/' + playlistID + '/tracks?offset=' + str(len(songIDs)), headers=headers)
        playlistData = json.loads(response2.content)
        for track in playlistData['items']:
            currentIDs.append(track['track']['id'])
            songdict[track['track']['id']] = {}
            songdict[track['track']['id']]['name'] = track['track']['name']
            songdict[track['track']['id']]['duration'] = track['track']['duration_ms']
            songdict[track['track']['id']]['artist'] = track['track']['artists'][0]['name']
            songdict[track['track']['id']]['album'] = track['track']['album']['name']
        songIDs.update(currentIDs)
        response3 = requests.get(url="https://api.spotify.com/v1/audio-features?ids=" + ','.join(currentIDs), headers=headers)
        audioData = json.loads(response3.content)
        for song in audioData['audio_features']:
            tempos.append(song['tempo'])
            songdict[song['id']]['tempo'] = song['tempo']
        print(len(songIDs))
    sorteddict = sorted(songdict.items(), key=lambda k_v: k_v[1]['tempo'])
    return songdict, sorteddict, tempos

def tempohist(tempos):
    plt.hist(tempos)
    plt.show()

def findClosest(tempo, songs):
    small = 1000
    for songID in songs.keys():
        if np.abs(songs[songID]['tempo'] - tempo) < small:
            small = np.abs(songs[songID]['tempo'] - tempo)
            minID = songID
    print(songs[minID])
    return minID

def queue(headers, songDict):
    current = getTempo()
    print(current)
    songID = findClosest(current, songDict)
    print(songID)
    requests.post(url='https://api.spotify.com/v1/me/player/queue?uri=spotify:track:' + songID, headers=headers)
    return songDict[songID]['duration']

def getTempo():
    #temporary random value returned
    tempo = np.random.normal(150, 35, 1)[0]
    return tempo
'''
def player(headers, songDict):
    while True:
        response = requests.get(url="https://api.spotify.com/v1/me/player/currently-playing", headers=headers)
        playing = json.loads(response.content)
        timeRemaining = playing["item"]["duration_ms"] - playing["progress_ms"]
        print(str(int(timeRemaining / 1000)) + " seconds left in song.")
        if timeRemaining < 15000:
            queue(headers, songDict)
        time.sleep(10)'''

def player(headers, songDict):
    #get current song progress, determine when there will be <30 seconds left in song
    remaining = (getCurrentProgress(headers) - 30000) / 1000
    if remaining > 0:
        time.sleep(remaining)
    
    while True:
        remaining = queue(headers, songDict)
        wait = (remaining) / 1000
        time.sleep(wait)


if __name__ == "__main__":
    username = sys.argv[1]
    scopes = "user-read-playback-state user-library-read user-modify-playback-state"
    token = getToken(username, '57893bf29f14448d97d7f8076e560b10', 'a2677090b9cb4fb0a118fa96bdedeef8', 'http://localhost:3000/callback/', scopes)
    headers = getHeaders(token)
    playlistID = getPlaylistID(headers)
    songDict, sortedDict, tempos = getPlaylistInfo(headers, playlistID)
    #tempohist(tempos)
    
    #queue(headers, songDict)
    player(headers, songDict)
