package com.example.demotest;

import android.content.Context;
import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;


import org.junit.Test;
import org.junit.runner.RunWith;

import java.security.SecureRandom;

import static org.junit.Assert.*;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class ExampleInstrumentedTest {
    @Test
    public void useAppContext() {
        // Context of the app under test.
//        Context appContext = InstrumentationRegistry.getTargetContext();

//        assertEquals("com.example.demotest1","1");
//        Log.i("@@","@@@@@@@@@:");
//        for (int i = 0; i < 10000000; ++i) {
//            AESUtil2 util2 = new AESUtil2();
//
//            byte[] key = generateRandom();
//
//            byte[] iv = generateRandom();
//
//            byte[] buf = generateRandom();
//
//
//            byte[] encode = util2.encryptECB(key, buf);
//            StringBuilder sb = new StringBuilder();
//            for (int j = 0; j < buf.length; ++j) {
//                sb.append(Integer.toHexString(buf[j]));
//            }
////            Log.i("@@", "@@@@@@@@encode:" + sb);
//
//            byte[] decocea = util2.decryptECB(key, encode);
//            StringBuilder sb2 = new StringBuilder();
//            for (int j = 0; j < decocea.length; ++j) {
//                sb2.append(Integer.toHexString(decocea[j]));
//            }
////            Log.i("@@", "@@@@@@@@decode:" + sb2);
//
////            assertArrayEquals(buf, decocea);
//            assertEquals("i:"+i,sb,sb2);
////            assertEquals();
//        }

    }

    static private byte[] generateRandom() {
        int len = (int) (Math.random() * 100);
        byte[] r = new byte[len];
        SecureRandom random = new SecureRandom();
        random.nextBytes(r);
        return r;
    }
}
