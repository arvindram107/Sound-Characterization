Sound-Characterization
======================

Algorithms to characterize the sounds of hindustani vocal, in C for pre-Win8 windows.
Project blog at http://chaelaa.wordpress.com

A little history:
We set up a recording studio in Allahabad (Northern India) which is a more recent center for hindustani. We learnt vocal till the point we could discern a few raags and a little bit of tabla. Enough to analyze the sound production process and to characterize the sound producers involved. Our recording space has significant feedback with marble floors and concerte parallel walls. We also subscribe to an approach to recording where we include the accoustics of our venue in the recording. Hence, we don't close mic and we use stereo micing. We try to make the micing and mixing arrangement as simple as possible giving maximum control to the performer. A "set it and forget it" type of approach that seems to work well for this kind of music. We interface with our electronics using a USB 4 input 2 output interface and we get at its bufferes through ASIO.

Next Steps:
The first version of this codebase will have some components that help you with characterizing a tanpura. This tanpura is a gents tanpura from Miraj, tuned to 275Hz with the common (pa, saa, saa, Saa) tuning. While we will hardcode everything to our hardware set up, since you have the source code, it should be relatively easy to modify it for other set ups. We will help you with that if needed. There is a driver version of ASIO for Windows that works on all common windows sound cards called ASIO4All. A good project for a new contributor would be to port the code to that driver so that it becomes usable on most Windows PCs.
