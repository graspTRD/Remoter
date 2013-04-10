package com.pccontroler.activity.connection;

import android.app.ActivityGroup;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;
import android.widget.LinearLayout;

import com.pccontroler.R;
import com.pccontroler.app.PCControler;

public class ConnectionMainActivity extends ActivityGroup
{
	private PCControler application;
	
	private Button buttonRefresh;
	
	private Button buttonNew;
	
	private LinearLayout layout;
	
	// protected LocalActivityManager mLocalActivityManager;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		this.application = (PCControler) this.getApplication();
		
		setContentView(R.layout.connectionlist);
		
		layout = (LinearLayout) findViewById(R.id.listbody);
		
		buttonRefresh = (Button) findViewById(R.id.refresh);
		
		// this.buttonRefresh.setOnClickListener(new
		// android.view.View.OnClickListener()
		// {
		//
		// @Override
		// public void onClick(View v)
		// {
		// int x = 0;
		// // buttonRefresh.setText("123");
		// // TODO Auto-generated method stub
		//
		// }
		// });
		
		this.startActivity("activity", new Intent(this, ConnectionListActivity.class));
		
	}
	
	private void startActivity(String tag, Intent intent)
	{
		
		final Window window = getLocalActivityManager().startActivity(tag, intent);
		
		final View decorView = window.getDecorView();
		
		if (decorView != null)
		{
			decorView.setVisibility(View.VISIBLE);
			
			decorView.setFocusableInTouchMode(true);
			
			((ViewGroup) decorView).setDescendantFocusability(ViewGroup.FOCUS_AFTER_DESCENDANTS);
			
			if (decorView.getParent() == null)
			{
				layout.addView(decorView, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT));
			}
			decorView.requestFocus();
		}
	}
	
}
