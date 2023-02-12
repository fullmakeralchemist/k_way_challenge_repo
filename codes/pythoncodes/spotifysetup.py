import spotipy
from spotipy.oauth2 import SpotifyOAuth


device_id = "" #add device ID from developers spotify
client_id = ""#add client ID from developers spotify
client_secret = "" #add client secret from developers spotify
redirect_uri = "http://localhost:8080"
scope = "user-read-playback-state,user-modify-playback-state"

sp = spotipy.Spotify(
        auth_manager=spotipy.SpotifyOAuth(
          client_id=client_id,
          client_secret=client_secret,
          redirect_uri=redirect_uri,    
          scope=scope, open_browser=False))


sp.start_playback(device_id=device_id ,uris=['spotify:track:1o7D1gLUgpFR3eJfIgpSUx'])#change song