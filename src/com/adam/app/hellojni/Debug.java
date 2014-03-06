package com.adam.app.hellojni;

import android.util.Log;

public class Debug {

    private static final boolean isDebug =  true;
    
    private static final String TAG = "Debug";
    
    static void printI(String str) {
        if(isDebug) Log.i(TAG, str);
    }
    
    
}
