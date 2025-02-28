package ru.igormayachenkov.flagman

import android.content.Context
import android.util.Log
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import ru.igormayachenkov.flagman.radio.Message
import ru.igormayachenkov.flagman.radio.RadioMan

object Captain {
    private val TAG = "myapp.Captain"

    data class State(
        val receiveMessagesCount:Int
    )
    private val _state = MutableStateFlow<State>(State(0))
    val state = _state.asStateFlow()

    // IMPLEMENTATION
    private val scope = CoroutineScope(Dispatchers.IO)
    private val radioMan = RadioMan()


    fun start(context: Context) {
        Log.d(TAG,"start")


        scope.launch {
            radioMan.run()
        }
    }

    fun sendMessage(){
        val msg = Message(time=System.currentTimeMillis(), data= ByteArray(0))
        scope.launch {
            radioMan.toSend.send(msg)
        }
    }

    @OptIn(ExperimentalCoroutinesApi::class)
    fun readReceivedMessages(){
        scope.launch {
            radioMan.received.apply {
                while(!isEmpty) {
                    val msg = receive()
                    _state.emit(State(state.value.receiveMessagesCount+1))
                }
            }
        }
    }
}