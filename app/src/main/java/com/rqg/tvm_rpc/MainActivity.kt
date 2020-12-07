package com.rqg.tvm_rpc

import android.Manifest
import android.content.Context
import android.os.Bundle
import android.os.PowerManager
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.tbruyelle.rxpermissions3.RxPermissions
import io.reactivex.rxjava3.android.schedulers.AndroidSchedulers
import kotlin.concurrent.thread

class MainActivity : AppCompatActivity() {
    companion object {
        private const val TAG = "MainActivity"
    }

    private var wakeLock: PowerManager.WakeLock? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        wakeLock = (getSystemService(Context.POWER_SERVICE) as PowerManager).run {
            newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "TVM_RPC:RUN").apply {
                acquire(Long.MAX_VALUE)
            }
        }

        RxPermissions(this)
            .request(Manifest.permission.WAKE_LOCK)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribe { granted ->
                if (granted) {

                    thread {
                        BridgeNative.runRPC()
                    }
                } else {
                    Toast.makeText(this, "permission deny", Toast.LENGTH_SHORT).show()
                }
            }
    }

    override fun onResume() {
        super.onResume()
        Log.d(TAG, "onResume() called")
    }

    override fun onPause() {
        super.onPause()
        Log.d(TAG, "onPause() called")
    }

    override fun onDestroy() {
        super.onDestroy()
        wakeLock?.release()
        Log.d(TAG, "onDestroy() called")
    }
}