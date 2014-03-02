/************************************************************
    dingdong.cmd

    REXX procedure to play bigben.wav via MMPM


    Marc E.E. van Woerkom, 12/93
 ************************************************************/


rc = call RxFuncAdd('mciRxInit', 'MCIAPI',,         /* register MCI REXX API */
                    'mciRxInit')
rc = call mciRxInit()                               /* and initialize it */


rc = mciRxSendString('open waveaudio',              /* open digital audio */
                     'alias wave shareable wait',,
                     'RetStr', '0', '0')

rc = mciRxSendString('load wave bigben.wav wait',,  /* load wav */
                     'RetStr', '0', '0')

rc = mciRxSendString('play wave wait',,                  /* play file */
                     'RetStr', '0', '0')

rc = mciRxSendString('close wave wait',,            /* close device "wave" */
                     'RetStr', '0', '0')


call mciRxExit

exit rc                                             /* exit with rc code */
