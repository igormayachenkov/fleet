package ru.igormayachenkov.flagman

import android.content.Context
import android.util.Log
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
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
            radioMan.received.collect { msg ->
                Log.d(TAG,"received message: $msg")
                _state.emit(
                    State(_state.value.receiveMessagesCount+1)
                )
            }
            Log.d(TAG,"finished")
        }
    }
}