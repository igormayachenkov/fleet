package ru.igormayachenkov.flagman.radio

import android.util.Log
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.channels.ReceiveChannel
import kotlinx.coroutines.channels.SendChannel
import kotlinx.coroutines.delay

class RadioMan {
    private val TAG = "myapp.RadioMan"

    private val _input  = Channel<Message>(capacity = 100)
    val toSend:SendChannel<Message> = _input

    private val _output = Channel<Message>(capacity = 100)
    val received:ReceiveChannel<Message> = _output

    suspend fun run(){
        while (true){
            // Get the message or wait
            Log.d(TAG,"waiting... for input")
            val msgToSend = _input.receive()

            // Process
            Log.d(TAG,"sending...  message: $msgToSend")
            delay(1_000)
            val msgReceived = Message(time = System.currentTimeMillis(), data=ByteArray(0))
            Log.d(TAG,"received message: $msgToSend")

            // Put to the queue
            _output.send(msgReceived)
        }
    }


//    val received = flow<Message> {
//        Log.d(TAG,"receive flow started")
//        var i = 0L
//        while(true){
//            delay(3_000)
//            i++
//            emit(Message(i,ByteArray(0)))
//        }
//    }
}