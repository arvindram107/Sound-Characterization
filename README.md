Sound-Characterization
======================

Algorithms to characterize the sounds of hindustani vocal, in C for pre-Win8 windows.
Project blog at http://chaelaa.wordpress.com

A little history:
We set up a recording studio in Allahabad (Northern India) which is a more recent center for hindustani. We learnt vocal till the point we could discern a few raags and a little bit of tabla. Enough to analyze the sound production process and to characterize the sound producers involved. Our recording space has significant feedback with marble floors and concerte parallel walls. We also subscribe to an approach to recording where we include the accoustics of our venue in the recording. Hence, we don't close mic and we use stereo micing. We try to make the micing and mixing arrangement as simple as possible giving maximum control to the performer. A "set it and forget it" type of approach that seems to work well for this kind of music. We interface with our electronics using a USB 4 input 2 output interface and we get at its bufferes through ASIO.

Next Steps:
The first version of this codebase will have some components that help you with characterizing a tanpura. Our tanpura is a gents tanpura from Miraj, tuned to 275Hz with the common (pa, saa, saa, Saa) tuning. Currently, we use a pair of AKG C414s in a coincident stereo arrangement with the sound-stage spread across the length of the tanpura. While we will hardcode everything to our hardware set up, since you have the source code, it should be relatively easy to modify it for other set ups. We will help you with that if you ask. All developers should join Steinberg's developer network and download their SDK as a way to get any ASIO queries answered. There is a driver version of ASIO for Windows that works on all common windows sound cards called ASIO4All. A good project for a new software contributor would be to port the code to that driver so that it becomes usable on most Windows PCs. Other bigger projects suited for more specialized contributors would be to port our codebase to the Mac and to iOS and Android. We plan to produce these in the future. On the hardware side, we plan to create an FPGA implementation and a custom ASIC implementation of our core algos, to be incorporated into gadgets.

The First Codebase:
I wrote this long ago. It's an adaptation of the ASIO sample app for Windows that ships with the SDK. That app has a DOS style interface. This one is our own variation of the classic Windows (pre-Win8) app. We go full-screen and we don't use the mouse in the interface design, only the keyboard for input. We expect through this project to build a version that has a sound based interface so that the app can be controlled straight by the musician in the studio. If you first go through the ASIO SDK and build and run the sample app on your hardware. You will then be in a good position to understand how the code works. Of course you will need some rudimentary knowledge of how a Windows app is structured. That you can get from MSDN documentation for the Win32 API. I haven't yet revised my bearings for this old code and am posting it as it is. To get it to build, you will have to modify project settings to reflect the location of the ASIO SDK and its various headers and libraries and source files. You will also have to discover the name of your ASIO driver and change that string in one of the top level files. The current code hardcodes for the settings found in our driver. You will have to re-adapt these setting fro your hardware. Note: this codebase has been built with an old MSVC 6.0 compiler. You may need to port the project configuration files to newer cmpilers.

A quick glance at the code reveals that you can start or stop the ASIO streaming process by pressing space bar anytime and exit the app using the 'h' key. The app plays back a center of soundstage version of a sinewave at 275HZ to act as a tuning drone. You set this output to play in your headphones. Then tune the tanpura and play it. The app records from the first two inputs to a file on disk. So this should get you started with your tanpura characterization.

The next version will enable you to do some simple surgery on the signals you have recorded starting with a visualization interface whose sound production can be calibrated with its signal display. This means you can tune out the latencies that normally exist in such systems for a real-time responce. Subsequently, we will build in tools to visualize the input tracks live in an effective way, with the same or similar signal surgery capabilities. After that we'll start building a sound based interface to this app to short out the role of the control room PC in the process. That will split our efforts into two tracks. One to better characterize the tanpura and the other to design and build sound based studio interfaces.


