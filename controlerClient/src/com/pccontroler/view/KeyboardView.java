package com.pccontroler.view;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;

import com.pccontroler.action.KeyboardAction;
import com.pccontroler.action.TextAction;
import com.pccontroler.activity.ControlActivity;
import com.pccontroler.app.PCControler;

public class KeyboardView extends Button
{
	private ControlActivity controlActivity;
	private PCControler application;
	private SharedPreferences preferences;
	
	public KeyboardView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		
		this.controlActivity = (ControlActivity) context;
		this.application = (PCControler) this.controlActivity.getApplication();
		this.preferences = application.getPreferences();
		
		setFocusable(true);
		setFocusableInTouchMode(true);
		
		OnClickListener lis = new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				InputMethodManager input = (InputMethodManager) getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
				input.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
			}
		};
		
		setOnClickListener(lis);
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		int unicode = event.getUnicodeChar();
		
		if (unicode == 0 && event.getKeyCode() == KeyEvent.KEYCODE_DEL)
		{
			unicode = KeyboardAction.UNICODE_BACKSPACE;
		}
		
		if (unicode != 0)
		{
			this.application.sendAction(new KeyboardAction(unicode));
		}
		return super.onKeyDown(keyCode, event);
	}
	
	public InputConnection onCreateInputConnection(EditorInfo outAttrs)
	{
		return new KeyBoardInputConnection(this, false);
	}
	
	public class KeyBoardInputConnection extends BaseInputConnection
	{
		
		public KeyBoardInputConnection(View targetView, boolean fullEditor)
		{
			super(targetView, fullEditor);
		}
		
		public boolean commitText(CharSequence text, int newCursorPosition)
		{
			application.sendAction(new TextAction(text.toString()));
			return true;
		}
	}
}
