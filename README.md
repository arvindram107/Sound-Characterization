Sound-Characterization
======================

Join us to try some retro style coding on some retro style music.
We design algorithms to characterize the sounds around us, in C for pre-Win8 windows.
See the blog of Khitchdee's Electronic Chaelaa project at http://chaelaa.wordpress.com. These are the first few, core technology building, steps in a more application focused project.

A little history:
We set up a recording studio in Allahabad (Northern India) which is a more recent center for hindustani. We learnt vocal till the point we could discern a few raags and a little bit of tabla. Enough to analyze the sound production process and to characterize the sound producers involved. Our recording space has significant feedback with marble floors and concrete parallel walls. We also subscribe to an approach to recording where we include the accoustics of our venue in the recording. Hence, we don't close mic and we use stereo micing. We try to make the micing and mixing arrangement as simple as possible giving maximum control to the performer. A "set it and forget it" type of approach that seems to work well for this kind of music. We interface with our electronics using a USB 4 input 2 output interface (the M-Audio Fast Track C600) and we get at its buffers through ASIO.

Next Steps:
The first version of this codebase will have some components that help you with characterizing a tanpura. Our tanpura is a gents tanpura from Miraj, tuned to 275Hz with the common (pa, saa, saa, Saa) tuning. Currently, we use a pair of AKG C414s in a coincident stereo arrangement with the sound-stage spread across the length of the tanpura. While we will hardcode everything to our hardware set up, since you have the source code, it should be relatively easy to modify it for other set ups. We will help you with that if you ask. All developers should join Steinberg's developer network and download their SDK as a way to get any ASIO queries answered. There is a driver version of ASIO for Windows that works on all common windows sound cards called ASIO4All. A good project for a new software contributor would be to port the code to that driver so that it becomes usable on most Windows PCs. Other bigger projects suited for more specialized contributors would be to port our codebase to the Mac and to iOS and Android. We plan to produce these in the future. On the hardware side, we plan to create an FPGA implementation and a custom ASIC implementation of our core algos, to be incorporated into gadgets. Some of the design decisions we make in the design of our code such as using C only and globals to pass parameters to all subroutines are influenced by these hardware goals.

The First Codebase:
I wrote this long ago. It's an adaptation of the ASIO sample app for Windows that ships with the SDK. That app has a DOS style interface. This one is our own variation of the classic Windows (pre-Win8) app. We go full-screen and we don't use the mouse in the interface design, only the keyboard for input. We expect through this project to build a version that has a sound based interface so that the app can be controlled straight by the musician in the studio. If you first go through the ASIO SDK and build and run the sample app on your hardware, you will then be in a good position to understand how the code works. Of course you will need some rudimentary knowledge of how a Windows app is structured. That you can get from MSDN documentation for the Win32 API. I haven't yet revised my bearings for this old code and am posting it as it is. To get it to build, you will have to modify project settings to reflect the location of the ASIO SDK and its various headers and libraries and source files. You will also have to discover the name of your ASIO driver and change that string in one of the top level files. When you install your sound interface's drivers, one on those drivers is the ASIO driver (assuming your sound interface supports ASIO). You can find that driver's string from the registry. The current code hardcodes for the settings found in our driver. You will have to re-adapt these setting for your hardware. Note: this codebase has been built with an old MSVC 6.0 compiler. You may need to port the project configuration files to newer compilers.

A quick glance at the code reveals that you can start or stop the ASIO streaming process by pressing space bar anytime and exit the app using the 'h' key. The app plays back a center of soundstage version of a sinewave at 275HZ to act as a tuning drone. You set this output to play in your headphones. Then tune the tanpura and play it. The app records from the first two inputs to a file on disk. So this should get you started with your tanpura characterization.

The next version will enable you to do some simple surgery on the signals you have recorded starting with a visualization interface whose sound production can be calibrated with its signal display. This means you can tune out the latencies that normally exist in such systems for a real-time responce. Subsequently, we will build in tools to visualize the input tracks live in an effective way, with the same or similar signal surgery capabilities. After that we'll start building a sound based interface to this app to short out the role of the control room PC in the process. That will split our efforts into two tracks. One to better characterize the tanpura and the other to design and build sound based studio interfaces.

We intend to release our first music piece, a ballad about Khitchdee called "Fat lie kebab -- it's a big fat lie" on Jan 14th. Our initial efforts will be focused on that until that date. It's tentatively got the following tracks in it: 1) A tanpura drone at 275Hz which we want in synth. 2) Some tabla loops in a eleven beat rhythm called step_eleven also in synth, 3) A vocal and acoustic guitar track that will be played live. There may be some multi-tracking involved.

To create the tanpura drone, we just need to tune a tanpura well to a played back 275Hz tone, then splice out a set of loops from the recording and in the playback, cross fade the ends. We need some controls for the location of the cross-fades and the identification of a good segment for splicing out. The tabla will also be a fixed length track that needs to be looped. We'll have to figure out a way to splice the tabla strip into a loop without any discernible distortions at the joint. We'll create signal surgery tools for the tanpura and the tabla.

The following steps are needed to achieve these results:
1. Revamp the code to either just play the tuning tone or switch it off and record on cue.
2. Create a function to read the recorded file and one to display it. Create a UX protocol for the display and editing and playback for the tanpura and implement it in a hard coded way for now. 
3. Do the same for the tabla.
4. Add multi-tracking with proper synchronization of synth tracks with the live track. In a later release, a proper calibration function may be added here to calibrate any given sound setup.
5. Encode the output in some appropriate format like .mp3

The tanpura characterizer's UX protocol:
On the input side, we don't use the mouse at all. Instead, for each protocol element, we design a mapping of the keyboard that maps different sets of keys to different input functions. Some mappings are very detailed in terms of how many different functions thay can support and are used by many different protocol elements. They keyboard input handler for the app loads one of many different UX protocol states. Each such state loads a set of UX protocol elements and maps their keyboard response functions to the desired set of actions for the differents sets of key in the input map. The desired action may perform some core action such as storing a file, or it may also display information back to the user. On the output side, we don't use windows and go full-screen and we use only a 400x240 sub-area in the center of the screen. Using this general UX protocol implementation architecture, we can now design the tanpura characterizer's specific UX protocol.

The basic steps that need be followed to design any UX protocol for a given interaction with a retro Khitchdee app are the same. This serves as a good small example that illustrates the steps involved. 

26/12/14

There's one constraint that we follow that relates to how time is organized during an interaction session with a retro Khitchdee app. We organize the timing of protocol events in sync with a rhythm. Currently we use an 11 steprhythm that we designed ourselves that goes like 1,2,3,4, 1,2,3 1,2 1, _. 11 is a prime number and we wanted to choose a rhythm that was distinct from all the other rhythms that we find around us, included those in music and in movies, ticking clocks, the sound of a horse's feet on the road as it runs. Having a distinct timing pattern helps keep out disturbances since they get cancelled out by the pattern. Of course this constraint is not neccasary and it's possible to build a retro Khitchdee UX protocol without conforming to any pre-determined time organization. However, this does have benefits when it comes to time management since an organized flow of time is slightly easier to manage. The level of predictability goes up. Our rhythm is called step_eleven and if you follow tabla terminology for describing a rhythm, it has 5 parts (called vibhaags in tabla) of 4,3,2,1 and 1 clock respectively. The last interval is meant to be a blank which marks the start of the next rhythm cycle. In tabla terminology, this is called a khaali and the baayan of the tabla does not boom during this time. Each of the other four parts starts with an emphasis which is called a taali in tabla which means clap. If you were to demonstrate step_eleven with just clapping, you would clap at each of the taalis, which means the first, fifth, eight and tenth clock. This taal (or rhythm) has 4 taalis and one khaali. This is rather unusual for a taal since the khaali is so short. A more typical taal such as teen taal has four equally spaced vibhaags of four clocks each. The entire third vibhaag is khaali hence in terms of ratios, 1/4 of the time is khaali and 3/4 has taalis. This is a more typical distribution. This uneven distribution makes step_eleven rather choppy. Intuitively, this choppiness should be good for a UX protocol that is trying to help its user focus on the app they are interacting with through this protocol. All actions in the protocol, both by the user and the app occur at the taalis and never in between. This is along the lines of conventions used in vocal music on laying out the music over a rhythm (we don't go off-beat yet). The description of the UX protocol then becomes a series of actions by the user or by the app that occur at the taalis of step_eleven. Of course we need a preamble in the protocol where the clock gets initialized and also ways to stop the clock or adjust it anytime during the session. In terms of roles played by the two agents in the conversation, the user plays the role of guru and the app the role of chaelaa (sidekick).

To design a UX protocol for some assisted activity, you help it's user identify what they will do and what they'd like their virtual chaelaa to do in response. You organize this description into a series of activities and then design a unique protocol for each activity. That description is a set of events occuring at the step_eleven taalis of a sequence of cycles. Each event is described in terms of what if anything the guru does to initiate some actions from the chaelaa and what the chaelaa does in response to a previous request by the guru. There are input side actions from the guru in terms of keystrokes made for a specific keyboard layout associated with the event and output and processor side actions in terms of the computational processing initiated by the chaelaa on the processor and the information displayed to the 400x240 screen.

Let's go through this process for the tanpura characterizer.
"To create the tanpura drone, we just need to tune a tanpura well to a played back 275Hz tone, then splice out a set of loops from the recording and in the playback, cross fade the ends. We need some controls for the location of the cross-fades and the identification of a good segment for splicing out."

Aside: We use a simple format to describe the UX protocol. For now, the protocol consists of a series of interactive segments. Segments can be free-flow, where there are no time-constraints or rhythm-bound. Describing a protocol hence consists of identifying the type, characteristics and sequence of interactive segments in the interaction. Later, as part of a separate project (Retro-coding) we will use this format to automatically generate the source code for a retro style Khitchdee app. This will greatly simplify the app building process and leave the programmer mainly with the identification of what they want the app to do for their user and only having to write the code for new functions added to the codebase.

A typical session with this app will involve the following:
1. Launching the app which plays the 275Hz tone into the headphone feed in the recording room.
2. Tuning the tanpura (in the recording room).
3. Pressing space bar to stop the tone and start the recording (in the control room).
4. Playing the tanpura for a while (in the recording room).
5. Pressing space bar to stop the recording and save it to a time-stamped file (in the control room)
Editing the loop in the control room
7. Identifying a good segment of the tanpura. Start and end points and fade-in and fade-out points need to be identified.
8. Playing back the loop into recording room headphones.
9. Tuning the tabla.
10. Starting the recording of the tabla sample.
11. Playing the tabla
12. Stopping the recording.
13. Identifying segments of the tabla to be used for the step_eleven loops to be used by the synth.
14. Playing the tanpura and the tabla into the headphones.
15. Tuning the guitar
16. Starting the recording of the song
17. Live track recording
18. Stopping the live track recording
19. Playing the tanpura, tabla and the live tracks, all synchronized properly into the headphones
20. Multi-tracking
21. Saving the 2-track mix-down output to a stereo file format.

Now, we need two things to build the system:

1. Let's now organize the session into a series of segments, each one represented by a distinct state machine, with states represented by a keyboard map and the actions to be made in response to the allowed key-strikes (from the map). Those actions would consist of executing some functions and drawing on the screen. 

2. Let's also identify the processing functions that need to be built to realize the system. 

As a first step, let's get the UI to work with place-holders for all the required functions. Then we can realize the behavior behind the UI. Let's design the grand state machine that will realize each of the segments in the session. That will fully define the UI.

Analyzing the list of activities in the session, we need to flesh out the following:
7. Identifying a good segment of the tanpura and looping it.
13. Identifying segments of the tabla to be used for the step_eleven loops to be used by the synth.
20. Multi-tracking

7a Calibrating out the playback & waveform display latency
7b Selecting the loop end-points
7c Selecting the fade interval
back to 7b or done (save the tanpura and loop).

13a Selecting a segment and testing it in step_eleven

20 Selecting which tracks to play and to record and their gains

7a(i) Measuring space-bar press system latency
7a(ii) The app generating and displaying a waveform with a sequence of spikes
7a(iii) Playing back the spike waveform and scrolling the waveform based on latency numbers predicted by ASIO. Listening to the sounds, looking at the display and tapping space bar on the spike. App displays location of space-bar tap relative to spike (while factoring in space-bar press system latencies) to identify residual latencies. Storing residual latency in a global.

7b(i) The app loading the stereo tanpura and displaying it.
7b(ii) Trimming out the ends.
7b(iii) Playing the tanpura. Tapping Caps Lock to identify gross start point and Enter to identify end point. App zooms in to gross segment, play it back again. ID process repeated, until user presses 'h'. 
7c(i) App loads currently chosen segment and default fade interval. Plays back the loop-joint on space bar. User changes the fade interval to tune it using the center strip, with Caps Lock for extreme reduction and enter for extreme increment. Repeats the preview process until satisfied (they press 'h')

13a Selecting a segment from the recorded tabla is slightly different than the one for the tanpura. In this case we need much greater accurracy in the location of the segment end-points. We also need a way of fine-tuning the playback. By default, the synth will take the designated (1,2,3,4) segment and use it to generate sub-segments for (1,2,3), (1,2) and (1). The step_eleven sequence will then be played as the origional segment sequenced with the 3 generated segments and a period of silence. For starters, we will only enable readjustment of the start and points of the origional segment for fine-tuning of the playback. Later will will see if we need to do any blends at the segment boundaries. After a coarse selection using the Caps Lock and Enter driven timing markers, the selection of these end points will be done mostly using visual cues since we know the visual are timing calibrated with the sound. The center strip can be used for this as was used for the fade interval adjustment.

20 Selecting which tracks to play and their gains
'h' key to select tanpura synth, Caps Lock or Enter to select tabla synth and space bar to select any live recorded track. With each of the synth dialogs, user sets level with center strip. With the live track dialog, user scrolls through the tracks, presses space bar to select, then sets level with a center strip. Tracks to be recorded are treated the same way. By default the to be recorded tracks always have maximum gain. 'h' key to exit live track dialog. Now space bar to start playback/recording, then space bar again to stop. App automatically stops playback at end of file. 'h' key to save the mix-down in a .wav and the mix parameters in a new multi-track file format. 

Now, let's scope out what major functions need to be implemented to get all this to work.

- Revision of the recording function. (to include time-stamping and content description)
- Waveform generator to generate a sequence of spikes.
- Scrolling and zooming waveform display
- Stored raw file player
- Tanpura synth (segment player with cross fading)
- step_eleven tabla synth
- Mixer

For starters, we won't do any streaming. Instead we'll load and keep all raw files entirely in memory. Since our song is pretty short, we won't have to worry about running out of memory.

Now we need to scope out the remaining tasks and see how we can deliver by the 14th.

28/12/14 Sunday off

29/12/14



