import requests
import json
import sys
from pprint import pprint
from matplotlib import pyplot as plt

def getHeaders(oAuth):
    headers = {
    'Accept': 'application/json',
    'Content-Type': 'application/json',
    'Authorization': 'Bearer ' + oAuth,
    }
    return headers

if __name__ == "__main__":
    token = sys.argv[1]
    headers = getHeaders(token)
    
    response = requests.get(url='https://api.spotify.com/v1/me/player', headers=headers)
    playback = json.loads(response.content)['context']['uri'].split(':')
    if playback[1] == 'playlist':
        playlistID = playback[2]
    songIDs = set()
    tempos = []
    while len(songIDs) % 100 == 0:
        currentIDs = []
        response2 = requests.get(url='https://api.spotify.com/v1/playlists/' + playlistID + '/tracks?offset=' + str(len(songIDs)), headers=headers)
        playlistData = json.loads(response2.content)
        for track in playlistData['items']:
            currentIDs.append(track['track']['id'])
        songIDs.update(currentIDs)
        response3 = requests.get(url="https://api.spotify.com/v1/audio-features?ids=" + ','.join(currentIDs), headers=headers)
        audioData = json.loads(response3.content)
        for song in audioData['audio_features']:
            tempos.append(song['tempo'])
        print(len(songIDs))
    
    plt.hist(tempos)
    plt.show()
    