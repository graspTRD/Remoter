package com.pccontroler.action;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class TextAction extends PCControlerAction
{
	public String text;
	
	public TextAction(String text)
	{
		this.text = text;
	}
	
	public static TextAction parse(DataInputStream dis) throws IOException
	{
		String text = dis.readUTF();
		
		return new TextAction(text);
	}
	
	public void toDataOutputStream(DataOutputStream dos) throws IOException
	{
		dos.writeByte(TEXT);
		dos.writeUTF(this.text);
	}
}
