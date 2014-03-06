package com.adam.app.hellojni;


public class JNISvr {
    
    // ---------------------------------------------------------
    // accessed by native methods
    // --------------------
    int data1 = 0; 
    static int data2 = 0;
    
        
    static {
        System.loadLibrary("adamjni");
    }
    

    private static onJNIListener mListener = null;
    
    public JNISvr() {
        Debug.printI("Construct");        
    }
    
    public void setOnJNIListener(onJNIListener listener) {
        
        mListener = listener;
    }
    
    
    public int getData1() {
        Debug.printI("getData1 function is called..");
                
        return data1;
    }
    
    public int getData2() {
        Debug.printI("getData2 function is called..");
                
        return data2;
    }
    
    
    public String getJNIString() {
        
        return _getJNIString();
    }
    
    public void testCallBack() {
        
        _testCallBack();
    }
    
    public void testStaticCallBack() {
        
        _testStaticCallBack();
    }
    
    public void setJavaData(int value) {
        
        _setJavaData(value);
    }
    
    
    public void setJavaStaticData(int value) {
        
        _setJavaStaticData(value);
    }

    public interface onJNIListener {
        
        void onNativeFunc1();
        
        void onNativeStaticFunc1();
        
    }
    
    
    // ---------------------------------------------------------
    // Java methods called from the native side
    // --------------------    
    public void nativeFunc1() {
        Debug.printI("nativeFunc1 function is called..");
        mListener.onNativeFunc1();
    }
    
    public static void nativeStaticFunc1() {
       Debug.printI("nativeStaticFunc1 function is called..");
       mListener.onNativeStaticFunc1();
    
    }    
    
    
    // ---------------------------------------------------------
    // Native methods called from the Java side
    // --------------------    
    private native final String _getJNIString();
    private native final void _testCallBack();
    private native static final void _testStaticCallBack();
    private native final void _setJavaData(int i);
    private native static final void _setJavaStaticData(int i);

}
