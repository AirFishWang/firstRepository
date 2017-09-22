package com.example.assettest;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class AssetCopyer {
    public  static void copyAllAssets(Context context,String assetpath, String destination)
    {
        copyAssetsToDst(context,assetpath,destination);
    }
    /**
     *
     * @param context :application context
     * @param srcPath :the path of source file
     * @param dstPath :the path of destination
     */
    private  static void copyAssetsToDst(Context context, String srcPath, String dstPath) {
        try {
            String fileNames[] =context.getAssets().list(srcPath);
            Log.e("dstPath", dstPath);
            for(String fileName : fileNames)
            {
                Log.e("fileName", fileName);
            }

            if (fileNames.length > 0)   //说明srcPath是一个目录
            {
                File file = new File(dstPath);
                file.mkdirs();
                for (String fileName : fileNames)
                {
                    if(srcPath!="")
                    {
                        copyAssetsToDst(context,srcPath + "/" + fileName,dstPath+"/"+fileName);
                    }else{
                        copyAssetsToDst(context, fileName,dstPath+"/"+fileName);
                    }
                }
            }else
            {
                InputStream is = context.getAssets().open(srcPath);
                FileOutputStream fos = new FileOutputStream(new File(dstPath));
                byte[] buffer = new byte[1024];
                int byteCount=0;
                while((byteCount=is.read(buffer))!=-1) {
                    fos.write(buffer, 0, byteCount);
                }
                fos.flush();//刷新缓冲区
                is.close();
                fos.close();
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
