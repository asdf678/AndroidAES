package com.example.demotest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import aes.AESUtil;

import java.security.SecureRandom;

public class MainActivity extends AppCompatActivity {
    String mResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        CommonConstant.load("pre");
//        Log.i("@@", "@@@@@@:" + CommonConstant.BASE_URL + '\n' + CommonConstant.BASE_URL_FOR_H5);


//        Log.i("@@", "@@@@@@@:" + new NDK().stringFromJNI());

        final EditText buf = findViewById(R.id.buf);
        final EditText key = findViewById(R.id.key);
        final EditText iv = findViewById(R.id.iv);
        final TextView encode = findViewById(R.id.encode);
        final TextView decode = findViewById(R.id.decode);
        findViewById(R.id.btn_encode).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final AESUtil aesUtil2 = new AESUtil(key.getText().toString());

                switch (((RadioGroup) findViewById(R.id.radio_group)).getCheckedRadioButtonId()) {
                    case R.id.ecb:
                        mResult = aesUtil2.encryptECB(buf.getText().toString());
                        encode.setText(new String(Base64.encode(mResult.getBytes(), Base64.DEFAULT)));
                        break;
                    case R.id.cbc:
                        mResult = aesUtil2.encryptCBC(buf.getText().toString(), iv.getText().toString());
                        encode.setText(new String(Base64.encode(mResult.getBytes(), Base64.DEFAULT)));
                        break;
                    case R.id.ctr:
                        mResult = aesUtil2.encryptCTR(buf.getText().toString(), iv.getText().toString());
                        encode.setText(new String(Base64.encode(mResult.getBytes(), Base64.DEFAULT)));
                        break;
                }


            }
        });
        findViewById(R.id.btn_decode).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mResult != null) {
                    final AESUtil aesUtil2 = new AESUtil(key.getText().toString());

                    switch (((RadioGroup) findViewById(R.id.radio_group)).getCheckedRadioButtonId()) {
                        case R.id.ecb:
                            decode.setText((aesUtil2.decryptECB(mResult)));
                            break;
                        case R.id.cbc:
                            decode.setText((aesUtil2.decryptCBC(mResult, iv.getText().toString())));

                            break;
                        case R.id.ctr:
                            decode.setText((aesUtil2.decryptCTR(mResult, iv.getText().toString())));

                            break;
                    }

                } else {
                    Toast.makeText(MainActivity.this, "请先加密！", Toast.LENGTH_SHORT).show();
                }


            }
        });


//        CTRTest();
//        CBCTest();
//        ECBTest();
        ((RadioGroup) findViewById(R.id.radio_group)).setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                mResult = null;
                encode.setText(null);
                decode.setText(null);
                switch (checkedId) {
                    case R.id.ecb:
                        findViewById(R.id.iv_title).setVisibility(View.GONE);
                        findViewById(R.id.iv).setVisibility(View.GONE);
                        break;
                    case R.id.cbc:
                        findViewById(R.id.iv_title).setVisibility(View.VISIBLE);
                        findViewById(R.id.iv).setVisibility(View.VISIBLE);
                        break;
                    case R.id.ctr:
                        findViewById(R.id.iv_title).setVisibility(View.VISIBLE);
                        findViewById(R.id.iv).setVisibility(View.VISIBLE);
                        break;
                }
            }
        });
        ((RadioButton) findViewById(R.id.ecb)).setChecked(true);
//        findViewById(R.id.btn_lib).setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                AESUtil aesUtil = new AESUtil();
//                final String key = "0123456789012345";
//                aesUtil.initialize(key);
//                String content = "你好";
//                {
//                    byte[] b = content.getBytes();
//
//                    StringBuilder sb = new StringBuilder();
//                    for (int i = 0; i < b.length; ++i) {
//                        sb.append(b[i]).append(',');
//                    }
//                    sb.deleteCharAt(sb.length() - 1);
//                    Log.i("@@", "@@@@@@@@@@@java content:" + sb.toString());
//                }
//
//                String encode = aesUtil.encryptECB(content);
//
//                {
//                    byte[] b = encode.getBytes();
//
//                    StringBuilder sb = new StringBuilder();
//                    for (int i = 0; i < b.length; ++i) {
//                        sb.append(b[i]).append(',');
//                    }
//                    sb.deleteCharAt(sb.length() - 1);
//                    Log.i("@@", "@@@@@@@@@@@java encode:" + sb.toString());
//                }
//
//                String decode = aesUtil.decryptECB(encode);
//
//                {
//                    byte[] b = decode.getBytes();
//
//                    StringBuilder sb = new StringBuilder();
//                    for (int i = 0; i < b.length; ++i) {
//                        sb.append(b[i]).append(',');
//                    }
//                    sb.deleteCharAt(sb.length() - 1);
//                    Log.i("@@", "@@@@@@@@@@@java decode:" + sb.toString());
//                }
//
//
//                Log.i("@@", "@@@@@@content:" + content + ",encode:" + encode + ",decode:" + decode);
//
//            }
//        });

//        findViewById(R.id.btn_random).setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//
//                new Thread() {
//                    @Override
//                    public void run() {
//                        super.run();
//                        for (int i = 0; i < 10000; ++i) {
//                            Log.i("@@", "@@@@@@@@Test:" + i);
//
//                            String key = new String(generateRandomKey());
//                            AESUtil util2 = new AESUtil(new String(key));
//
////                            Log.i("@@", "@@@@@@@@Test:" + i);
//
//                            String iv = new String(generateRandomIv());
//
//                            String buf = new String(generateRandomBuf());
//
////                            printHex(buf);
//                            String encode = null;
//                            String decocea = null;
//                            switch (((RadioGroup) findViewById(R.id.radio_group)).getCheckedRadioButtonId()) {
//                                case R.id.ecb:
//                                    encode = util2.encryptECB((buf));
//                                    decocea = util2.decryptECB(encode);
//                                    break;
//                                case R.id.cbc:
//                                    encode = util2.encryptCBC((buf), (iv));
//                                    decocea = util2.decryptCBC(encode, (iv));
//                                    break;
//                                case R.id.ctr:
//                                    encode = util2.encryptCTR((buf), (iv));
//                                    decocea = util2.decryptCTR((encode), (iv));
//                                    break;
//                            }
////                            printHex(decocea);
////                            if (!MainActivity.equals(buf, decocea)) {
////                                printDetails(buf, decocea);
////                                throw new IllegalArgumentException("Not equals");
////                            }
//                            if (!encode.equals(decocea)) {
//                                Log.i("@@", "@@@@@@@content:" + buf);
//                                Log.i("@@", "@@@@@@@aencode:" + encode);
//                                Log.i("@@", "@@@@@@@adecode:" + decocea);
//
//                                throw new IllegalArgumentException("Not equals,content:" + buf + ",encode:" + encode + ",decode:" + decocea);
//                            }
//
//                            Log.i("@@", "@@@@@@@@--------------------------------------------");
//
//
//                        }
//                    }
//                }.start();
//
//
//            }
//        });


    }

    private static void printHex(byte[] buf) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < buf.length; ++i) {
            sb.append(Integer.toHexString(buf[i]));
        }
        Log.i("@@", "@@@@@@@@Hex:" + sb);
    }

    private static void printInt(byte[] buf) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < buf.length; ++i) {
            sb.append(buf[i]);
        }
        Log.i("@@", "@@@@@@@@Int:" + sb);
    }

    private static void printStr(byte[] buf) {
        String s = new String(buf);
        Log.i("@@", "@@@@@@@@Str:" + s);
    }

    private static boolean equals(byte[] buf, byte[] decoce) {
        if (buf.length != decoce.length) {
//            throw new IllegalArgumentException("Not Equals");
            return false;
        }

        for (int i = 0; i < buf.length; ++i) {
            if (buf[i] != decoce[i]) {
//                throw new IllegalArgumentException("Not Equals");
                return false;
            }
        }
        return true;


    }


    private static void printDetails(byte[] buf, byte[] decoce) {
        Log.i("@@", "@@@@@buf:" + createArrString(buf));
        Log.i("@@", "@@@@@dec:" + createArrString(decoce));

    }

    private static String createArrString(byte[] buf) {
        StringBuilder sb = new StringBuilder();
        sb.append('[');
        for (int i = 0; i < buf.length; ++i) {
            sb.append(buf[i]).append(',');
        }
        sb.deleteCharAt(sb.length() - 1);
        sb.append(']');
        return sb.toString();
    }

    static private byte[] generateRandomBuf() {
        return fillByteArr();
    }

    static private byte[] generateRandomKey() {
        return fillByteArr();
    }

    static private byte[] generateRandomIv() {
        return fillByteArr();
    }

    static byte[] fillByteArr() {

        int len = (int) (Math.random() * 100);
        byte[] r = new byte[len];
        SecureRandom random = new SecureRandom();
        random.nextBytes(r);
        for (int i = 0; i < r.length; ++i) {
            if (r[i] == 0) {
                r[i] = 30;
            }
        }

        return r;
    }
}
