package aes;

import android.util.Base64;

public class AESUtil {
    static {
        System.loadLibrary("aes-lib");
    }

    private final byte[] mKey;

    public AESUtil(String mKey) {
        this.mKey = mKey.getBytes();
    }


    /*
     * ecb:
     * AES256: key_length :32,in_length:16,out_length:16
     * AES192: key_length :24,in_length:16,out_length:16
     * AES128: key_length :16,in_length:16,out_length:16
     * */
    public String encryptECB( String content) {
        return new String(Base64.encode(encryptECB(mKey, content.getBytes()), Base64.DEFAULT));
    }

    public String decryptECB( String content) {
        return new String(decryptECB(mKey,Base64.decode( content.getBytes(), Base64.DEFAULT)));
    }

    byte[] encryptECB(byte[] key, byte[] buf) {
        return nativeEncryptECB(key, buf);
    }

    byte[] decryptECB(byte[] key, byte[] buf) {
        return nativeDecryptECB(key, buf);
    }

    native private byte[] nativeEncryptECB(byte[] key, byte[] buf);

    native private byte[] nativeDecryptECB(byte[] key, byte[] buf);

//    native public String nativeDecryptECB2Str(byte[] key, byte[] buf);

    /*
     * ctr:
     * AES256: key_length :32,in_length:64,out_length:64,iv_length:16
     * AES192: key_length :24,in_length:64,out_length:64,iv_length:16
     * AES128: key_length :16,in_length:64,out_length:64,iv_length:16
     * */

    public String encryptCTR( String buf, String iv) {
        return new String(Base64.encode(encryptCTR(mKey, buf.getBytes(), iv.getBytes()), Base64.DEFAULT));
    }

    public String decryptCTR( String buf, String iv) {
        return new String(decryptCTR(mKey, Base64.decode(buf.getBytes(), Base64.DEFAULT), iv.getBytes()));

    }

    byte[] encryptCTR(byte[] key, byte[] buf, byte[] iv) {
        return nativeEncryptCTR(key, buf, iv);
    }

    byte[] decryptCTR(byte[] key, byte[] buf, byte[] iv) {
        return nativeDecryptCTR(key, buf, iv);

    }

    native private byte[] nativeEncryptCTR(byte[] key, byte[] buf, byte[] iv);

    native private byte[] nativeDecryptCTR(byte[] key, byte[] buf, byte[] iv);


    /*
     * cbc
     * AES256: key_length :32,in_length:64,out_length:64,iv_length:16
     * AES192: key_length :24,in_length:64,out_length:64,iv_length:16
     * AES128: key_length :16,in_length:64,out_length:64,iv_length:16
     * */

    public String encryptCBC( String buf, String iv) {
        return new String(Base64.encode(encryptCBC(mKey, buf.getBytes(), iv.getBytes()), Base64.DEFAULT));
    }

    public String decryptCBC( String buf, String iv) {
        return new String(decryptCBC(mKey, Base64.decode(buf.getBytes(), Base64.DEFAULT), iv.getBytes()));
    }

    byte[] encryptCBC(byte[] key, byte[] buf, byte[] iv) {
        return nativeEncryptCBC(key, buf, iv);
    }

    byte[] decryptCBC(byte[] key, byte[] buf, byte[] iv) {
        return nativeDecryptCBC(key, buf, iv);
    }

    native private byte[] nativeEncryptCBC(byte[] key, byte[] buf, byte[] iv);

    native private byte[] nativeDecryptCBC(byte[] key, byte[] buf, byte[] iv);


}
