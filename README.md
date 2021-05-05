# Rhythm

Rhythm is a product that seeks to synchronize a user’s music tempo to their cadence as they are working out, specifically on stationary workout equipment in a gym. 

The physical device tracks a user’s movement to determine their workout cadence and then sends this data to the Rhythm mobile application via Bluetooth, where the app utilizes the Spotify API to actively queue songs that match the tempo of the workout.


The code for Rhythm involves a combination of embedded software and an Android mobile application.
## Embedded software
The embedded software comprises of code to control the functionality of a STM32L053 and its interactions with peripherals including Bluetooth Low Energy RN4020 via UART, a Battery Babysitter using I2C, and an ICM20602 IMU device using I2C.
Embedded code can be found in path: Rhythm/embedded/test/477_BluetoothInterface/
## Application
The application was developed using Android Studio, primarily consisting of a UI, Spotify API request interface, and Bluetooth scanning and receiving data. Three primary sources were utilized in the development of the application. For basic setup and reference, [Android Developers](https://developer.android.com/training/basics/firstapp), for Spotify API [reference](https://tolkiana.com/how-to-use-spotifys-sdk-in-kotlin/) and [code](https://github.com/tolkiana/spotify-player/blob/master/app/src/main/java/com/tolkiana/spotifyplayer/SpotifyService.kt) and for BLE [reference](https://punchthrough.com/android-ble-guide/) and [code](https://github.com/PunchThrough/ble-starter-android).



This project was completed by Ethan Glaser, Weston Donati-Leach, Douglas Yu, and Noah Rodriguez as part of ECE 477 - Digital Systems Senior Design - at Purdue University during the Spring 2021 semester. For more information about the project and team, please visit https://engineering.purdue.edu/477grp17/index.html.
