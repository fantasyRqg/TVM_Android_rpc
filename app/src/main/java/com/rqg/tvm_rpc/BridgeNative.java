package com.rqg.tvm_rpc;

/**
 * * Created by rqg on 2020/11/19.
 */
class BridgeNative {
    static {
        System.loadLibrary("tvm_glue");
    }


    public static native void runRPC(int port, String trackerAddr, String customAddr);
}
