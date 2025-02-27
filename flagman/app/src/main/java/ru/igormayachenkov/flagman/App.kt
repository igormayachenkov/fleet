package ru.igormayachenkov.flagman

import android.app.Application
import android.util.Log

class App : Application(){
    private val TAG = "myapp.App"

    override fun onCreate() {
        super.onCreate()
        Log.w(TAG, "onCreate")

        Captain.start(this)

    }

}