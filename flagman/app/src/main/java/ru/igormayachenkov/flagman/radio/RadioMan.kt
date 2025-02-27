package ru.igormayachenkov.flagman.radio

import android.util.Log
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.flow

class RadioMan {
    private val TAG = "myapp.RadioMan"

    val received = flow<Message> {
        Log.d(TAG,"receive flow started")
        var i = 0L
        while(true){
            delay(3_000)
            i++
            emit(Message(i,ByteArray(0)))
        }
    }
}