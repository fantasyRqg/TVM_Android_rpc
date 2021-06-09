package com.rqg.tvm_rpc

import android.Manifest
import android.content.Context
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.tbruyelle.rxpermissions3.RxPermissions
import io.reactivex.rxjava3.android.schedulers.AndroidSchedulers
import kotlinx.android.synthetic.main.activity_main.*
import kotlin.concurrent.thread
import kotlin.system.exitProcess

class MainActivity : AppCompatActivity() {
    companion object {
        private const val TAG = "MainActivity"

        private const val SP_PORT = "SP_PORT"
        private const val SP_TRACKER = "SP_TRACKER"
        private const val SP_CUSTOM = "SP_CUSTOM"
    }

    private val seps by lazy { getSharedPreferences("nnnnn", Context.MODE_PRIVATE) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        if (seps.contains(SP_PORT)) {
            etPort.setText(seps.getInt(SP_PORT, 9090).toString())
        }
        etTrackerAddr.setText(seps.getString(SP_TRACKER, null))
        etCustomAddr.setText(seps.getString(SP_CUSTOM, null))


        btnStart.setOnClickListener {
            val port = etPort.text.toString().toInt()
            val tracker = etTrackerAddr.text.toString()
            val customAddr = etCustomAddr.text.toString()

            val trackerParts = tracker.split(":")
            Log.d(TAG, "onCreate: ${trackerParts}")
            if (trackerParts.size != 2) {
                Toast.makeText(this, "invalid tracker address", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }

            val cmd_tracker = "('${trackerParts[0]}', ${trackerParts[1]})"

            val cmd_custom = if (customAddr.isNotBlank()) {
                "\"$customAddr\""
            } else {
                ""
            }

            seps.edit()
                .putInt(SP_PORT, port)
                .putString(SP_TRACKER, tracker)
                .putString(SP_CUSTOM, customAddr)
                .apply()

            RxPermissions(this)
                .request(Manifest.permission.WAKE_LOCK)
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe { granted ->
                    if (granted) {
                        thread {
                            BridgeNative.runRPC(port, cmd_tracker, cmd_custom)
                        }
                        btnStart.isEnabled = false
                    } else {
                        Toast.makeText(this, "permission deny", Toast.LENGTH_SHORT).show()
                    }
                }

        }

        btnExit.setOnClickListener {
            exitProcess(0)
        }
    }
}