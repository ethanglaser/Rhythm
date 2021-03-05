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

enum class PlayingState {
    PAUSED, PLAYING, STOPPED
}

object SpotifyService {
    private const val playlistDict = "{\"6C4LXC9UFH1IKiHYOp0BiJ\": {\"name\": \"Sunday Bloody Sunday - Remastered 2008\", \"duration\": 279440, \"artist\": \"U2\", \"album\": \"War (Remastered)\", \"tempo\": 101.174}, \"5QTxFnGygVM4jFQiBovmRo\": {\"name\": \"(Don't Fear) The Reaper\", \"duration\": 308120, \"artist\": \"Blue \\u00d6yster Cult\", \"album\": \"Agents Of Fortune\", \"tempo\": 141.466}, \"7LRMbd3LEoV5wZJvXT1Lwb\": {\"name\": \"T.N.T.\", \"duration\": 214666, \"artist\": \"AC/DC\", \"album\": \"High Voltage\", \"tempo\": 126.366}, \"19Shlms2uTnOjIUg50TXzd\": {\"name\": \"Never Going Back Again - 2004 Remaster\", \"duration\": 134400, \"artist\": \"Fleetwood Mac\", \"album\": \"Rumours (Super Deluxe)\", \"tempo\": 176.083}, \"10Nmj3JCNoMeBQ87uw5j8k\": {\"name\": \"Dani California\", \"duration\": 282160, \"artist\": \"Red Hot Chili Peppers\", \"album\": \"Stadium Arcadium\", \"tempo\": 96.184}, \"4DMKwE2E2iYDKY01C335Uw\": {\"name\": \"Carry on Wayward Son\", \"duration\": 323000, \"artist\": \"Kansas\", \"album\": \"Leftoverture (Expanded Edition)\", \"tempo\": 126.755}, \"39C5FuZ8C8M0QI8CrMsPkR\": {\"name\": \"Foreplay / Long Time\", \"duration\": 467640, \"artist\": \"Boston\", \"album\": \"Boston\", \"tempo\": 118.693}, \"37ZJ0p5Jm13JPevGcx4SkF\": {\"name\": \"Livin' On A Prayer\", \"duration\": 249293, \"artist\": \"Bon Jovi\", \"album\": \"Slippery When Wet\", \"tempo\": 122.511}, \"4bHsxqR3GMrXTxEPLuK5ue\": {\"name\": \"Don't Stop Believin'\", \"duration\": 250986, \"artist\": \"Journey\", \"album\": \"Escape\", \"tempo\": 118.852}, \"1mea3bSkSGXuIRvnydlB5b\": {\"name\": \"Viva La Vida\", \"duration\": 242373, \"artist\": \"Coldplay\", \"album\": \"Viva La Vida or Death and All His Friends\", \"tempo\": 138.015}, \"6VoIBz0VhCyz7OdEoRYDiA\": {\"name\": \"Say It Ain't So\", \"duration\": 258826, \"artist\": \"Weezer\", \"album\": \"Weezer\", \"tempo\": 151.779}, \"40riOy7x9W7GXjyGp4pjAv\": {\"name\": \"Hotel California - 2013 Remaster\", \"duration\": 391376, \"artist\": \"Eagles\", \"album\": \"Hotel California (2013 Remaster)\", \"tempo\": 147.125}, \"5lDriBxJd22IhOH9zTcFrV\": {\"name\": \"Dirty Little Secret\", \"duration\": 193653, \"artist\": \"The All-American Rejects\", \"album\": \"Move Along\", \"tempo\": 143.85}, \"2l57cfmCnOkwNX1tky02n1\": {\"name\": \"Move Along\", \"duration\": 238880, \"artist\": \"The All-American Rejects\", \"album\": \"Move Along\", \"tempo\": 109.864}, \"1VuBmEauSZywQVtqbxNqka\": {\"name\": \"Beautiful Day\", \"duration\": 246400, \"artist\": \"U2\", \"album\": \"All That You Can't Leave Behind\", \"tempo\": 136.279}, \"08mG3Y1vljYA6bvDt4Wqkj\": {\"name\": \"Back In Black\", \"duration\": 255493, \"artist\": \"AC/DC\", \"album\": \"Back In Black\", \"tempo\": 188.386}, \"5fqcIHU6DhQtFKVO5XSdQs\": {\"name\": \"Roam\", \"duration\": 295133, \"artist\": \"The B-52's\", \"album\": \"Cosmic Thing\", \"tempo\": 134.937}, \"5GorCbAP4aL0EJ16frG2hd\": {\"name\": \"Boulevard of Broken Dreams\", \"duration\": 262333, \"artist\": \"Green Day\", \"album\": \"Greatest Hits: God's Favorite Band\", \"tempo\": 167.06}, \"2lwwrWVKdf3LR9lbbhnr6R\": {\"name\": \"Float On\", \"duration\": 208466, \"artist\": \"Modest Mouse\", \"album\": \"Good News For People Who Love Bad News\", \"tempo\": 100.975}, \"4xh7W7tlNMIczFhupCPniY\": {\"name\": \"Go Your Own Way - 2004 Remaster\", \"duration\": 223613, \"artist\": \"Fleetwood Mac\", \"album\": \"Rumours (Super Deluxe)\", \"tempo\": 135.448}, \"5cfYurP1XKldMBtvBBJiTs\": {\"name\": \"Ramblin' Man\", \"duration\": 288506, \"artist\": \"Allman Brothers Band\", \"album\": \"Dreams\", \"tempo\": 179.396}, \"6GG73Jik4jUlQCkKg9JuGO\": {\"name\": \"The Middle\", \"duration\": 165853, \"artist\": \"Jimmy Eat World\", \"album\": \"Bleed American\", \"tempo\": 162.152}, \"1JdKrFyoU05abww0Zv0ayQ\": {\"name\": \"That's What You Get\", \"duration\": 220480, \"artist\": \"Paramore\", \"album\": \"Riot!\", \"tempo\": 131.094}, \"7cy1bEJV6FCtDaYpsk8aG6\": {\"name\": \"China Grove\", \"duration\": 195306, \"artist\": \"The Doobie Brothers\", \"album\": \"The Captain and Me\", \"tempo\": 145.624}, \"2b9lp5A6CqSzwOrBfAFhof\": {\"name\": \"Crazy Train\", \"duration\": 296200, \"artist\": \"Ozzy Osbourne\", \"album\": \"Blizzard of Ozz (Expanded Edition)\", \"tempo\": 137.571}, \"4RvWPyQ5RL0ao9LPZeSouE\": {\"name\": \"Everybody Wants To Rule The World\", \"duration\": 251488, \"artist\": \"Tears For Fears\", \"album\": \"Songs From The Big Chair (Super Deluxe Edition)\", \"tempo\": 112.067}, \"5EWPGh7jbTNO2wakv8LjUI\": {\"name\": \"Free Bird\", \"duration\": 547106, \"artist\": \"Lynyrd Skynyrd\", \"album\": \"Pronounced' Leh-'Nerd 'Skin-'Nerd\", \"tempo\": 118.223}, \"5E89Izp4YhPyNShoxiOJ1u\": {\"name\": \"Rock & Roll Band\", \"duration\": 180346, \"artist\": \"Boston\", \"album\": \"Boston\", \"tempo\": 136.743}, \"5UWwZ5lm5PKu6eKsHAGxOk\": {\"name\": \"Everlong\", \"duration\": 250546, \"artist\": \"Foo Fighters\", \"album\": \"The Colour And The Shape\", \"tempo\": 158.066}, \"0LHItNoPqjxWcbvQzw9NIv\": {\"name\": \"Saturday Night\\u2019s Alright (For Fighting) - Remastered 2014\", \"duration\": 295466, \"artist\": \"Elton John\", \"album\": \"Goodbye Yellow Brick Road (Remastered / Deluxe Edition)\", \"tempo\": 152.875}, \"56awWXMn89K8w6sQMvmE75\": {\"name\": \"New York Minute - Live On MTV, 1994\", \"duration\": 397666, \"artist\": \"Eagles\", \"album\": \"Hell Freezes Over (Remaster 2018)\", \"tempo\": 63.873}, \"1jJci4qxiYcOHhQR247rEU\": {\"name\": \"Kids\", \"duration\": 302840, \"artist\": \"MGMT\", \"album\": \"Oracular Spectacular\", \"tempo\": 122.961}, \"6ADSaE87h8Y3lccZlBJdXH\": {\"name\": \"With Or Without You - Remastered\", \"duration\": 295515, \"artist\": \"U2\", \"album\": \"The Joshua Tree (Super Deluxe)\", \"tempo\": 110.171}, \"2374M0fQpWi3dLnB54qaLX\": {\"name\": \"Africa\", \"duration\": 295893, \"artist\": \"TOTO\", \"album\": \"Toto IV\", \"tempo\": 92.718}, \"5nNmj1cLH3r4aA4XDJ2bgY\": {\"name\": \"September\", \"duration\": 214826, \"artist\": \"Earth, Wind & Fire\", \"album\": \"The Eternal Dance\", \"tempo\": 125.941}, \"3G69vJMWsX6ZohTykad2AU\": {\"name\": \"One\", \"duration\": 276186, \"artist\": \"U2\", \"album\": \"Achtung Baby (Deluxe Edition)\", \"tempo\": 181.305}, \"6wpGqhRvJGNNXwWlPmkMyO\": {\"name\": \"I Still Haven't Found What I'm Looking For\", \"duration\": 277476, \"artist\": \"U2\", \"album\": \"The Joshua Tree (Super Deluxe)\", \"tempo\": 100.894}, \"57bgtoPSgt236HzfBOd8kj\": {\"name\": \"Thunderstruck\", \"duration\": 292880, \"artist\": \"AC/DC\", \"album\": \"The Razors Edge\", \"tempo\": 133.52}, \"2SiXAy7TuUkycRVbbWDEpo\": {\"name\": \"You Shook Me All Night Long\", \"duration\": 210173, \"artist\": \"AC/DC\", \"album\": \"Back In Black\", \"tempo\": 127.361}, \"69QHm3pustz01CJRwdo20z\": {\"name\": \"Hells Bells\", \"duration\": 312293, \"artist\": \"AC/DC\", \"album\": \"Back In Black\", \"tempo\": 106.767}, \"3ZffCQKLFLUvYM59XKLbVm\": {\"name\": \"Wake Me up When September Ends\", \"duration\": 285653, \"artist\": \"Green Day\", \"album\": \"American Idiot\", \"tempo\": 104.98}, \"64yrDBpcdwEdNY9loyEGbX\": {\"name\": \"21 Guns\", \"duration\": 321093, \"artist\": \"Green Day\", \"album\": \"21st Century Breakdown\", \"tempo\": 159.779}, \"4VgEM12OeaN3GyBV487Me7\": {\"name\": \"Holiday\", \"duration\": 232680, \"artist\": \"Green Day\", \"album\": \"Greatest Hits: God's Favorite Band\", \"tempo\": 146.525}, \"5DiXcVovI0FcY2s0icWWUu\": {\"name\": \"Mr. Jones\", \"duration\": 272293, \"artist\": \"Counting Crows\", \"album\": \"August And Everything After\", \"tempo\": 141.621}, \"4ccM2xBxicGigjLqt6A0YY\": {\"name\": \"Accidentally In Love\", \"duration\": 188613, \"artist\": \"Counting Crows\", \"album\": \"Accidentally In Love\", \"tempo\": 138.0}, \"7s4JCyY8YIZSsKvtwStBQ6\": {\"name\": \"Rain King\", \"duration\": 255773, \"artist\": \"Counting Crows\", \"album\": \"August And Everything After\", \"tempo\": 78.592}, \"6hTcuIQa0sxrrByu9wTD7s\": {\"name\": \"Born to Run\", \"duration\": 269920, \"artist\": \"Bruce Springsteen\", \"album\": \"Born To Run\", \"tempo\": 146.347}, \"2Y90nL1ohB4sgYELDs7uNx\": {\"name\": \"Glory Days\", \"duration\": 254733, \"artist\": \"Bruce Springsteen\", \"album\": \"Born In The U.S.A.\", \"tempo\": 117.486}, \"4RCWB3V8V0dignt99LZ8vH\": {\"name\": \"Hey There Delilah\", \"duration\": 232533, \"artist\": \"Plain White T's\", \"album\": \"All That We Needed\", \"tempo\": 103.971}, \"0wVluBsVAVzBKrqspuCcwR\": {\"name\": \"Check Yes, Juliet\", \"duration\": 220146, \"artist\": \"We The Kings\", \"album\": \"We The Kings\", \"tempo\": 166.795}, \"44aTAUBF0g6sMkMNE8I5kd\": {\"name\": \"Every Little Thing She Does Is Magic\", \"duration\": 260573, \"artist\": \"The Police\", \"album\": \"Ghost In The Machine (Remastered 2003)\", \"tempo\": 81.909}, \"5tVA6TkbaAH9QMITTQRrNv\": {\"name\": \"Free Fallin'\", \"duration\": 256000, \"artist\": \"Tom Petty\", \"album\": \"Full Moon Fever\", \"tempo\": 84.383}, \"0iOZM63lendWRTTeKhZBSC\": {\"name\": \"Mrs. Robinson - From \\\"The Graduate\\\" Soundtrack\", \"duration\": 244026, \"artist\": \"Simon & Garfunkel\", \"album\": \"Bookends\", \"tempo\": 92.033}, \"5ChkMS8OtdzJeqyybCc9R5\": {\"name\": \"Billie Jean\", \"duration\": 293826, \"artist\": \"Michael Jackson\", \"album\": \"Thriller 25 Super Deluxe Edition\", \"tempo\": 117.046}, \"2Fs18NaCDuluPG1DHGw1XG\": {\"name\": \"Life is a Highway\", \"duration\": 276320, \"artist\": \"Rascal Flatts\", \"album\": \"Me And My Gang\", \"tempo\": 103.027}, \"0GONea6G2XdnHWjNZd6zt3\": {\"name\": \"Summer Of '69\", \"duration\": 216053, \"artist\": \"Bryan Adams\", \"album\": \"Reckless (30th Anniversary / Deluxe Edition)\", \"tempo\": 139.131}, \"42Lq6GBO5KUSNeSgJsOoke\": {\"name\": \"Panama\", \"duration\": 212640, \"artist\": \"Van Halen\", \"album\": \"Best of Volume 1\", \"tempo\": 141.257}, \"423o3ZHIaBtGXyhF1uH41a\": {\"name\": \"Walk of Life\", \"duration\": 252466, \"artist\": \"Dire Straits\", \"album\": \"Brothers in Arms\", \"tempo\": 172.404}, \"1QEEqeFIZktqIpPI4jSVSF\": {\"name\": \"More Than a Feeling\", \"duration\": 285133, \"artist\": \"Boston\", \"album\": \"Boston\", \"tempo\": 108.736}, \"47i0RQi76b6UUCIBDgu2TR\": {\"name\": \"Supernatural Superserious\", \"duration\": 203880, \"artist\": \"R.E.M.\", \"album\": \"Accelerate\", \"tempo\": 125.079}, \"6K4t31amVTZDgR3sKmwUJJ\": {\"name\": \"The Less I Know The Better\", \"duration\": 216320, \"artist\": \"Tame Impala\", \"album\": \"Currents\", \"tempo\": 116.879}, \"2X485T9Z5Ly0xyaghN73ed\": {\"name\": \"Let It Happen\", \"duration\": 467586, \"artist\": \"Tame Impala\", \"album\": \"Currents\", \"tempo\": 125.012}, \"7i6r9KotUPQg3ozKKgEPIN\": {\"name\": \"Seven Nation Army\", \"duration\": 231733, \"artist\": \"The White Stripes\", \"album\": \"Elephant\", \"tempo\": 123.881}, \"3d9DChrdc6BOeFsbrZ3Is0\": {\"name\": \"Under the Bridge\", \"duration\": 264306, \"artist\": \"Red Hot Chili Peppers\", \"album\": \"Blood Sugar Sex Magik (Deluxe Edition)\", \"tempo\": 84.581}, \"57Xjny5yNzAcsxnusKmAfA\": {\"name\": \"Reptilia\", \"duration\": 219753, \"artist\": \"The Strokes\", \"album\": \"Room On Fire\", \"tempo\": 157.992}, \"2hnMS47jN0etwvFPzYk11f\": {\"name\": \"Santeria\", \"duration\": 182746, \"artist\": \"Sublime\", \"album\": \"Sublime\", \"tempo\": 90.807}, \"3MpK9vnxxgYvh0CNeGvx6G\": {\"name\": \"On the Road Again\", \"duration\": 148106, \"artist\": \"Willie Nelson\", \"album\": \"Forrest Gump - The Soundtrack\", \"tempo\": 111.281}, \"6cr6UDpkjEaMQ80OjWqEBQ\": {\"name\": \"Sultans of Swing\", \"duration\": 350400, \"artist\": \"Dire Straits\", \"album\": \"Dire Straits\", \"tempo\": 148.217}, \"6hx2ebU1GSzW7aUePCqCT0\": {\"name\": \"Fortunate Son\", \"duration\": 140773, \"artist\": \"Creedence Clearwater Revival\", \"album\": \"Willy And The Poor Boys (40th Anniversary Edition)\", \"tempo\": 132.754}, \"0A4PZuepTcIQVvA5m7R0M1\": {\"name\": \"Don't You (Forget About Me) - Remastered\", \"duration\": 263973, \"artist\": \"Simple Minds\", \"album\": \"Celebrate (Greatest Hits)\", \"tempo\": 133.019}, \"21MsRs5ShUpwYsr6xfuPyH\": {\"name\": \"Jack & Diane\", \"duration\": 254493, \"artist\": \"John Mellencamp\", \"album\": \"American Fool (Remastered)\", \"tempo\": 103.94}, \"2g7gviEeJr6pyxO7G35EWQ\": {\"name\": \"Paradise By the Dashboard Light\", \"duration\": 508333, \"artist\": \"Meat Loaf\", \"album\": \"Bat Out Of Hell\", \"tempo\": 173.381}, \"2xYlyywNgefLCRDG8hlxZq\": {\"name\": \"Take Me Home, Country Roads - Rerecorded\", \"duration\": 197813, \"artist\": \"John Denver\", \"album\": \"The John Denver Collection, Vol. 1: Take Me Home Country Roads\", \"tempo\": 164.267}, \"7Ar4G7Ci11gpt6sfH9Cgz5\": {\"name\": \"Listen to the Music\", \"duration\": 227266, \"artist\": \"The Doobie Brothers\", \"album\": \"Toulouse Street\", \"tempo\": 105.929}, \"27L8sESb3KR79asDUBu8nW\": {\"name\": \"Stacy's Mom\", \"duration\": 197986, \"artist\": \"Fountains Of Wayne\", \"album\": \"Welcome Interstate Managers\", \"tempo\": 118.015}, \"7snQQk1zcKl8gZ92AnueZW\": {\"name\": \"Sweet Child O' Mine\", \"duration\": 356066, \"artist\": \"Guns N' Roses\", \"album\": \"Appetite For Destruction\", \"tempo\": 125.303}, \"7N3PAbqfTjSEU1edb2tY8j\": {\"name\": \"Jump - 2015 Remaster\", \"duration\": 241599, \"artist\": \"Van Halen\", \"album\": \"1984 (Remastered)\", \"tempo\": 129.981}, \"3B4q6KbHbGV51HO3GznBFF\": {\"name\": \"What I Got\", \"duration\": 170906, \"artist\": \"Sublime\", \"album\": \"Sublime\", \"tempo\": 95.721}, \"30g3R8mv8qyLKv1A0cAFil\": {\"name\": \"Brown Eyed Girl\", \"duration\": 183520, \"artist\": \"Van Morrison\", \"album\": \"Brown Eyed Girl (Digital 45)\", \"tempo\": 148.882}, \"2dhJM2skHC9D41gHj34Kgw\": {\"name\": \"You\\u2019re The Best Thing About Me\", \"duration\": 225400, \"artist\": \"U2\", \"album\": \"Songs Of Experience (Deluxe Edition)\", \"tempo\": 117.023}, \"359XicDhyrKbqx8U9NQFPM\": {\"name\": \"No Line On The Horizon\", \"duration\": 252400, \"artist\": \"U2\", \"album\": \"No Line On The Horizon\", \"tempo\": 99.973}, \"47z5v7hkOTuvb82F0FWALC\": {\"name\": \"Magnificent\", \"duration\": 324013, \"artist\": \"U2\", \"album\": \"No Line On The Horizon\", \"tempo\": 114.973}, \"4CUoLQrUSVFlcxSrkWr1Rs\": {\"name\": \"Unknown Caller\", \"duration\": 362853, \"artist\": \"U2\", \"album\": \"No Line On The Horizon\", \"tempo\": 104.021}, \"2vVuKnnoYkA8HfKSK0oNbH\": {\"name\": \"I'll Go Crazy If I Don't Go Crazy Tonight\", \"duration\": 253946, \"artist\": \"U2\", \"album\": \"No Line On The Horizon\", \"tempo\": 103.056}, \"6B7HkOXoB3c0MoXk7C9i07\": {\"name\": \"Breathe\", \"duration\": 300306, \"artist\": \"U2\", \"album\": \"No Line On The Horizon\", \"tempo\": 116.926}, \"4Rxnw6luSLQh6cegmsAmsV\": {\"name\": \"I Will Follow - Remastered 2008\", \"duration\": 217386, \"artist\": \"U2\", \"album\": \"Boy\", \"tempo\": 155.151}, \"0Uj7uKXKg4qaRSYL2KiJ1B\": {\"name\": \"Stories For Boys - Remastered 2008\", \"duration\": 182440, \"artist\": \"U2\", \"album\": \"Boy\", \"tempo\": 155.021}, \"1gVVSmhWjci8l9Vrsl6HaS\": {\"name\": \"Vertigo\", \"duration\": 193520, \"artist\": \"U2\", \"album\": \"How To Dismantle An Atomic Bomb\", \"tempo\": 140.083}, \"2b6pyyrxfP79LhV2hLtTm4\": {\"name\": \"Miracle Drug\", \"duration\": 234013, \"artist\": \"U2\", \"album\": \"How To Dismantle An Atomic Bomb\", \"tempo\": 124.939}, \"6fzcnVO0X2cpYjsHk19GJo\": {\"name\": \"Sometimes You Can't Make It On Your Own\", \"duration\": 305080, \"artist\": \"U2\", \"album\": \"How To Dismantle An Atomic Bomb\", \"tempo\": 96.0}, \"2R3wzFFjNYc1z3PSoO8FNV\": {\"name\": \"City Of Blinding Lights\", \"duration\": 345706, \"artist\": \"U2\", \"album\": \"How To Dismantle An Atomic Bomb\", \"tempo\": 139.172}, \"0ExjS2TkTWgmJdoXCa7rNx\": {\"name\": \"Crumbs From Your Table\", \"duration\": 299186, \"artist\": \"U2\", \"album\": \"How To Dismantle An Atomic Bomb\", \"tempo\": 174.067}, \"6ml01NkC9AfM4OICOtRxEY\": {\"name\": \"Original Of The Species\", \"duration\": 274120, \"artist\": \"U2\", \"album\": \"How To Dismantle An Atomic Bomb\", \"tempo\": 173.803}, \"1H5QOvCduwR93MWvlm0zks\": {\"name\": \"Yahweh\", \"duration\": 261573, \"artist\": \"U2\", \"album\": \"How To Dismantle An Atomic Bomb\", \"tempo\": 126.014}, \"0RHPoliIwT6ddbPugZNitX\": {\"name\": \"Elevation\", \"duration\": 226133, \"artist\": \"U2\", \"album\": \"All That You Can't Leave Behind\", \"tempo\": 110.022}, \"4vQZzwQq2jPCGgCbfdGv5p\": {\"name\": \"Peace On Earth\", \"duration\": 286266, \"artist\": \"U2\", \"album\": \"All That You Can't Leave Behind\", \"tempo\": 101.982}, \"5p9XWUdvbUzmPCukOmwoU3\": {\"name\": \"Suddenly I See\", \"duration\": 201706, \"artist\": \"KT Tunstall\", \"album\": \"Eye To The Telescope\", \"tempo\": 100.38}, \"6J2rMSRhgb4HuX6dWgM3nJ\": {\"name\": \"I Melt with You\", \"duration\": 251573, \"artist\": \"Modern English\", \"album\": \"After the Snow\", \"tempo\": 155.582}, \"7h1YqA5MZrRxmkUFpukRcp\": {\"name\": \"Where The Streets Have No Name - Remastered\", \"duration\": 336613, \"artist\": \"U2\", \"album\": \"The Joshua Tree\", \"tempo\": 125.799}, \"4vvj5hekeftc4l64fLmOYJ\": {\"name\": \"New Year's Day - Remastered 2008\", \"duration\": 335613, \"artist\": \"U2\", \"album\": \"War (Remastered)\", \"tempo\": 133.468}, \"17b3qMO75c61kDebXzK5pl\": {\"name\": \"Gloria - Remastered 2008\", \"duration\": 252720, \"artist\": \"U2\", \"album\": \"October (Remastered)\", \"tempo\": 145.896}, \"0ofHAoxe9vBkTCp2UQIavz\": {\"name\": \"Dreams - 2004 Remaster\", \"duration\": 257800, \"artist\": \"Fleetwood Mac\", \"album\": \"Rumours (Super Deluxe)\", \"tempo\": 120.151}, \"55mQhobuwtY7lfLAXylg1k\": {\"name\": \"You Make Loving Fun - 2004 Remaster\", \"duration\": 216626, \"artist\": \"Fleetwood Mac\", \"album\": \"Rumours (Super Deluxe)\", \"tempo\": 126.576}, \"05oETzWbd4SI33qK2gbJfR\": {\"name\": \"Rhiannon\", \"duration\": 252773, \"artist\": \"Fleetwood Mac\", \"album\": \"Fleetwood Mac\", \"tempo\": 129.012}, \"4qN7nSAVTjPfOOz0wkRQpM\": {\"name\": \"In Your Eyes - 2012 Remaster\", \"duration\": 330373, \"artist\": \"Peter Gabriel\", \"album\": \"So (2012 Remastered)\", \"tempo\": 89.284}, \"1CM1wOqD2AIjt2MWd31LV2\": {\"name\": \"Solsbury Hill\", \"duration\": 261440, \"artist\": \"Peter Gabriel\", \"album\": \"Peter Gabriel 1: Car (Remastered Version)\", \"tempo\": 102.457}, \"4xUlOxwGkdhsQuirAp5RfV\": {\"name\": \"Moonshadow\", \"duration\": 172533, \"artist\": \"Yusuf / Cat Stevens\", \"album\": \"Teaser And The Firecat\", \"tempo\": 130.658}, \"5n1amEDwVKucAGMqePk9yN\": {\"name\": \"Young Turks - 2008 Remaster\", \"duration\": 302413, \"artist\": \"Rod Stewart\", \"album\": \"The Definitive Rod Stewart\", \"tempo\": 161.143}, \"451GvHwY99NKV4zdKPRWmv\": {\"name\": \"Banana Pancakes\", \"duration\": 191906, \"artist\": \"Jack Johnson\", \"album\": \"In Between Dreams\", \"tempo\": 114.303}, \"4NtUY5IGzHCaqfZemmAu56\": {\"name\": \"Dancing Queen\", \"duration\": 230693, \"artist\": \"ABBA\", \"album\": \"Arrival\", \"tempo\": 100.812}, \"215wUTQQUo2PElJFEFoB0d\": {\"name\": \"Wheel in the Sky\", \"duration\": 252240, \"artist\": \"Journey\", \"album\": \"Infinity\", \"tempo\": 105.191}, \"2nLtzopw4rPReszdYBJU6h\": {\"name\": \"Numb\", \"duration\": 185586, \"artist\": \"Linkin Park\", \"album\": \"Meteora\", \"tempo\": 110.018}, \"3fH4KjXFYMmljxrcGrbPj9\": {\"name\": \"Don't You (Forget About Me)\", \"duration\": 263040, \"artist\": \"Simple Minds\", \"album\": \"Once Upon A Time (Super Deluxe)\", \"tempo\": 111.346}, \"1SWmFiFSIBoDbQJjNKC7SR\": {\"name\": \"Against The Wind\", \"duration\": 333720, \"artist\": \"Bob Seger\", \"album\": \"Against The Wind\", \"tempo\": 109.618}, \"4s6LhHAV5SEsOV0lC2tjvJ\": {\"name\": \"California Dreamin' - Single Version\", \"duration\": 162373, \"artist\": \"The Mamas & The Papas\", \"album\": \"If You Can Believe Your Eyes & Ears\", \"tempo\": 112.367}, \"5qBqBdfTEIWJwAS0Jm2F5R\": {\"name\": \"Turn! Turn! Turn! (To Everything There Is a Season)\", \"duration\": 229360, \"artist\": \"The Byrds\", \"album\": \"Turn! Turn! Turn!\", \"tempo\": 124.474}, \"0PP933bHgvV0eD19dG3ms5\": {\"name\": \"Here I Go Again 87 - 2017 Remastered Version\", \"duration\": 275693, \"artist\": \"Whitesnake\", \"album\": \"Whitesnake (30th Anniversary Remaster)\", \"tempo\": 90.172}, \"3dh2LlmeMqKJbzn2WUgt3d\": {\"name\": \"Pride (In The Name Of Love) - Remastered 2009\", \"duration\": 228426, \"artist\": \"U2\", \"album\": \"The Unforgettable Fire (Remastered)\", \"tempo\": 105.701}, \"6ooluO7DiEhI1zmK94nRCM\": {\"name\": \"Take Me Out\", \"duration\": 237026, \"artist\": \"Franz Ferdinand\", \"album\": \"Franz Ferdinand\", \"tempo\": 104.545}, \"3QO8W45i5w0WJF8AQtY1KN\": {\"name\": \"This Is the End (For You My Friend)\", \"duration\": 192066, \"artist\": \"Anti-Flag\", \"album\": \"For Blood And Empire\", \"tempo\": 200.074}, \"7pebKpj33kDJxhKwqDH8pe\": {\"name\": \"Darkest Days\", \"duration\": 160533, \"artist\": \"Good Riddance\", \"album\": \"My Republic\", \"tempo\": 190.099}, \"0QYZaNoR7hTOSH1DkRGQxu\": {\"name\": \"Equador is Lovely This Time Of Year\", \"duration\": 225026, \"artist\": \"Inkwell\", \"album\": \"These Stars Are Monsters\", \"tempo\": 155.019}, \"0v8F6BHzAQrD9MWQcX3jbj\": {\"name\": \"In The End\", \"duration\": 228280, \"artist\": \"Black Veil Brides\", \"album\": \"Wretched and Divine: The Story Of The Wild Ones Ultimate Edition\", \"tempo\": 140.02}, \"573latfTMO7SpGtStVrQx5\": {\"name\": \"Harlem\", \"duration\": 163626, \"artist\": \"New Politics\", \"album\": \"A Bad Girl In Harlem\", \"tempo\": 133.97}, \"1nmZ8yqKkfooOuYvtFctDp\": {\"name\": \"Heat Of The Moment\", \"duration\": 227626, \"artist\": \"Asia\", \"album\": \"Asia\", \"tempo\": 136.232}, \"0aym2LBJBk9DAYuHHutrIl\": {\"name\": \"Hey Jude - Remastered 2015\", \"duration\": 425653, \"artist\": \"The Beatles\", \"album\": \"1 (Remastered)\", \"tempo\": 147.207}, \"3G7tRC24Uh09Hmp1KZ7LQ2\": {\"name\": \"I'm a Believer - 2006 Remaster\", \"duration\": 167373, \"artist\": \"The Monkees\", \"album\": \"More of The Monkees (Deluxe Edition)\", \"tempo\": 80.106}, \"6lrQo6KAYvb92MGk6ZuZlt\": {\"name\": \"Say You Love Me\", \"duration\": 249946, \"artist\": \"Fleetwood Mac\", \"album\": \"Fleetwood Mac\", \"tempo\": 128.068}, \"5ihS6UUlyQAfmp48eSkxuQ\": {\"name\": \"Landslide\", \"duration\": 199493, \"artist\": \"Fleetwood Mac\", \"album\": \"Fleetwood Mac\", \"tempo\": 159.375}, \"0xpBr84T3FTm9j4D1MdPtk\": {\"name\": \"Invisible Touch - 2007 Remaster\", \"duration\": 209586, \"artist\": \"Genesis\", \"album\": \"Invisible Touch\", \"tempo\": 131.037}, \"2ouURa1AIXp3AvkS52Jry5\": {\"name\": \"Every Morning\", \"duration\": 219866, \"artist\": \"Sugar Ray\", \"album\": \"14:59\", \"tempo\": 109.916}, \"6y4OjcEz7LMrdVGODrcAIv\": {\"name\": \"Forever Young\", \"duration\": 248000, \"artist\": \"Rod Stewart\", \"album\": \"Out of Order\", \"tempo\": 101.941}, \"3OqgbMEUghCAG5IyTxtrfG\": {\"name\": \"A Sort Of Homecoming - Live\", \"duration\": 243600, \"artist\": \"U2\", \"album\": \"Wide Awake In America\", \"tempo\": 125.74}, \"6t1FIJlZWTQfIZhsGjaulM\": {\"name\": \"Video Killed The Radio Star\", \"duration\": 253800, \"artist\": \"The Buggles\", \"album\": \"The Age Of Plastic\", \"tempo\": 131.037}, \"4KoecuyOpZaNFZ0UqVsllc\": {\"name\": \"Follow Me\", \"duration\": 218439, \"artist\": \"Uncle Kracker\", \"album\": \"Double Wide\", \"tempo\": 105.014}, \"6ihL9TjfRjadfEePzXXyVF\": {\"name\": \"Gives You Hell\", \"duration\": 213106, \"artist\": \"The All-American Rejects\", \"album\": \"When The World Comes Down\", \"tempo\": 100.008}, \"7e89621JPkKaeDSTQ3avtg\": {\"name\": \"Sweet Home Alabama\", \"duration\": 283800, \"artist\": \"Lynyrd Skynyrd\", \"album\": \"Second Helping\", \"tempo\": 97.798}, \"215JYyyUnrJ98NK3KEwu6d\": {\"name\": \"Soak Up The Sun\", \"duration\": 292306, \"artist\": \"Sheryl Crow\", \"album\": \"C'Mon C'Mon\", \"tempo\": 119.996}, \"0cJPLFrlV7TTCyPLupHzcH\": {\"name\": \"Won't Get Fooled Again - Original Album Version\", \"duration\": 511400, \"artist\": \"The Who\", \"album\": \"Who's Next\", \"tempo\": 135.027}, \"5oQcOu1omDykbIPSdSQQNJ\": {\"name\": \"1985\", \"duration\": 193000, \"artist\": \"Bowling For Soup\", \"album\": \"A Hangover You Don't Deserve\", \"tempo\": 119.974}, \"6XUHsYE38CEbYunT983O9G\": {\"name\": \"Give A Little Bit\", \"duration\": 248173, \"artist\": \"Supertramp\", \"album\": \"Even In The Quietest Moments\", \"tempo\": 90.767}, \"6tmslRyHPI7dyTe8rAHcbQ\": {\"name\": \"Come On Eileen\", \"duration\": 273933, \"artist\": \"Dexys Midnight Runners\", \"album\": \"Good Morning Music: Singing in the Shower\", \"tempo\": 108.097}, \"4d6eqRtpDX7tydHJGDZUBQ\": {\"name\": \"She Drives Me Crazy\", \"duration\": 215773, \"artist\": \"Fine Young Cannibals\", \"album\": \"The Raw & The Cooked\", \"tempo\": 108.804}, \"5JqMWnxVKjJZ2NNOEux46S\": {\"name\": \"Punkrocker - feat. Iggy Pop\", \"duration\": 246786, \"artist\": \"Teddybears\", \"album\": \"Soft Machine (U.S. Version)\", \"tempo\": 141.859}, \"6iX1f3r7oUJnMbGgQ2gx1j\": {\"name\": \"867-5309 / Jenny\", \"duration\": 226200, \"artist\": \"Tommy Tutone\", \"album\": \"Tommy Tutone - 2\", \"tempo\": 137.657}, \"3Zwu2K0Qa5sT6teCCHPShP\": {\"name\": \"Thnks fr th Mmrs\", \"duration\": 203506, \"artist\": \"Fall Out Boy\", \"album\": \"Infinity On High\", \"tempo\": 154.837}, \"5DnT9a5IM3eMjKgXTWVJvi\": {\"name\": \"Have You Ever Seen The Rain\", \"duration\": 160333, \"artist\": \"Creedence Clearwater Revival\", \"album\": \"Pendulum\", \"tempo\": 116.122}, \"1Jj6MF0xDOMA3Ut2Z368Bx\": {\"name\": \"Time After Time\", \"duration\": 243066, \"artist\": \"Cyndi Lauper\", \"album\": \"She's So Unusual: A 30th Anniversary Celebration (Deluxe Edition)\", \"tempo\": 130.439}, \"49NDegc7RQyvq6mhAs06p6\": {\"name\": \"Only Time Will Tell\", \"duration\": 285000, \"artist\": \"Asia\", \"album\": \"Asia\", \"tempo\": 125.52}, \"4gvea7UlDkAvsJBPZAd4oB\": {\"name\": \"The Boys Of Summer\", \"duration\": 288733, \"artist\": \"Don Henley\", \"album\": \"Building The Perfect Beast\", \"tempo\": 176.941}, \"6dGnYIeXmHdcikdzNNDMm2\": {\"name\": \"Here Comes The Sun - Remastered 2009\", \"duration\": 185733, \"artist\": \"The Beatles\", \"album\": \"Abbey Road (Remastered)\", \"tempo\": 129.171}, \"4OV0sRTFrZLXaZZ1sS9hLn\": {\"name\": \"Sirius - Chicago Bulls Theme Song\", \"duration\": 115280, \"artist\": \"The Alan Parsons Project\", \"album\": \"Eye In The Sky (Expanded Edition)\", \"tempo\": 111.401}, \"1gFNm7cXfG1vSMcxPpSxec\": {\"name\": \"Ob-La-Di, Ob-La-Da - Remastered 2009\", \"duration\": 188960, \"artist\": \"The Beatles\", \"album\": \"The Beatles (Remastered)\", \"tempo\": 113.059}, \"0BCPKOYdS2jbQ8iyB56Zns\": {\"name\": \"Clocks\", \"duration\": 307879, \"artist\": \"Coldplay\", \"album\": \"A Rush of Blood to the Head\", \"tempo\": 130.97}, \"01r3Yf8J8xjUH8Ut9CI0e8\": {\"name\": \"You, Me and the Bourgeoisie\", \"duration\": 204413, \"artist\": \"The Submarines\", \"album\": \"Honeysuckle Weeks\", \"tempo\": 127.056}, \"08oRPVeAXWNnRZUzNJpYsv\": {\"name\": \"Abraham, Martin And John\", \"duration\": 200400, \"artist\": \"Dion\", \"album\": \"Dion\", \"tempo\": 146.994}, \"6485cn8SoHq6qPdmeQcw4x\": {\"name\": \"Let Me Take You Home Tonight\", \"duration\": 286680, \"artist\": \"Boston\", \"album\": \"Boston\", \"tempo\": 76.908}, \"7Ewz6bJ97vUqk5HdkvguFQ\": {\"name\": \"Heaven\", \"duration\": 243360, \"artist\": \"Bryan Adams\", \"album\": \"Reckless (30th Anniversary / Deluxe Edition)\", \"tempo\": 140.059}, \"1D1nixOVWOxvNfWi0UD7VX\": {\"name\": \"Unwritten\", \"duration\": 259333, \"artist\": \"Natasha Bedingfield\", \"album\": \"Unwritten\", \"tempo\": 100.011}, \"3Q4WeJmzxuDpzMu9QjQqbM\": {\"name\": \"Big Girls Don't Cry (Personal)\", \"duration\": 268120, \"artist\": \"Fergie\", \"album\": \"The Dutchess\", \"tempo\": 113.082}, \"4oDZ5L8izBals6jKBJDBcX\": {\"name\": \"Your Love\", \"duration\": 216466, \"artist\": \"The Outfield\", \"album\": \"Play Deep\", \"tempo\": 129.537}, \"57XSd8FypQPCxJewkFXIwU\": {\"name\": \"The Unforgettable Fire - Remastered 2009\", \"duration\": 294000, \"artist\": \"U2\", \"album\": \"The Unforgettable Fire (Remastered)\", \"tempo\": 136.594}, \"3gdewACMIVMEWVbyb8O9sY\": {\"name\": \"Rocket Man (I Think It's Going To Be A Long, Long Time)\", \"duration\": 281613, \"artist\": \"Elton John\", \"album\": \"Honky Chateau\", \"tempo\": 136.571}, \"6ZtrGCcn38kGImt2GPFbJB\": {\"name\": \"Shot In The Dark\", \"duration\": 185133, \"artist\": \"AC/DC\", \"album\": \"Shot In The Dark\", \"tempo\": 114.065}, \"4lI5ov3iO3wzXY1Fz3Ho7i\": {\"name\": \"Bad - Live\", \"duration\": 480333, \"artist\": \"U2\", \"album\": \"Wide Awake In America\", \"tempo\": 101.979}}"
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
            Log.d("Playlist ID:", it.uri)
        }
    }

    fun getCurrentTrackImage(handler: (Bitmap) -> Unit)  {
        getCurrentTrack {
            getImage(it.imageUri) {
                handler(it)
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
    fun sendGetRequest(userName: String, password: String) {

        var reqParam = URLEncoder.encode("username", "UTF-8") + "=" + URLEncoder.encode(
                userName,
                "UTF-8"
        )
        reqParam += "&" + URLEncoder.encode("password", "UTF-8") + "=" + URLEncoder.encode(
                password,
                "UTF-8"
        )

        val mURL = URL("<Yout API Link>?" + reqParam)

        with(mURL.openConnection() as HttpURLConnection) {
            // optional default is GET
            requestMethod = "GET"

            println("URL : $url")
            println("Response Code : $responseCode")

            BufferedReader(InputStreamReader(inputStream)).use {
                val response = StringBuffer()

                var inputLine = it.readLine()
                while (inputLine != null) {
                    response.append(inputLine)
                    inputLine = it.readLine()
                }
                it.close()
                println("Response : $response")
            }
        }
    }

    fun executeCommandLine() {
        val comd = "curl -X 'GET' 'https://api.spotify.com/v1/playlists/2Y9WARZQEcDiWrYItwSe7d' -H 'Accept: application/json' -H 'Content-Type: application/json' -H 'Authorization: Bearer BQAUb2EALqMApwkJjwuFaAvMyzp7lPNzUjE9nMNproyWH2b_J3PQ9J0jbOBAyf22r0pXaJe215cw1Tv4ZaULLVgIggvPgnAneq0gyb0TALCSjdNeP_H8B09ZiExrubsE_oGV3cZShgfzzSNlCZi4ZXWlhjXqtH8HlJDYqWU4fZfMT4RoiUV8VY0t-LdkuL1sZiTkZruf-gh3ejvCrIEvHI9Wh9UIG2aUmJ2g7eSF7Ws9hm0-UGUpeUw5_RAJm_wZLeFOH0m0DSM47_TrJf-plUzoSA'"
        val process = Runtime.getRuntime().exec(comd)
        Log.d("STUFF", process.inputStream.toString())
    }

    fun jsonHandler(filename: String) {
        val gson = GsonBuilder().create()
        val type: Type = object : TypeToken<Map<String?, Song?>?>() {}.type
        val dict: Map<String, Song> = gson.fromJson(playlistDict, type)
        Log.d("playlist", dict.toString())
        rhythm(dict)
    }

    fun rhythm(dict: Map<String, Song>) {
        val random = Random()
        val gson = GsonBuilder().create()
        var first = dict.entries.elementAt(random.nextInt(dict.size))
        Log.d("a", first.toString())
        play(first.key.toString())

        var wait = first.value.duration - 30000
        while(1 == 1) {
            Thread.sleep(wait.toLong())
            var tempo = random.nextGaussian() * 35 + 150
            Log.d("TEMPO", tempo.toString())
            var closestsong = ""
            var closestvalue = 100.0
            for ((k, v) in dict) {
                if (abs(tempo - v.tempo) < closestvalue) {
                    closestvalue = abs(tempo - v.tempo)
                    closestsong = k
                    wait = v.duration
                }
            }
            Log.d("SONG", dict[closestsong].toString())

            queue(closestsong)
        }
    }


}