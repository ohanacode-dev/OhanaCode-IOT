package com.ohanacode.iotbrowser;

import android.content.Context;

import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

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
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();

<<<<<<< HEAD:IOTBrowser/app/src/androidTest/java/com/ohanacode/iotbrowser/ExampleInstrumentedTest.java
        assertEquals("com.ohanacode.iotbrowser", appContext.getPackageName());
=======
        assertEquals("com.ohanacode.ociotbrowser", appContext.getPackageName());
>>>>>>> New IOT app version:OC-IOT-Browser/app/src/androidTest/java/com/ohanacode-dev/oc-iot-browser/ExampleInstrumentedTest.java
    }
}
