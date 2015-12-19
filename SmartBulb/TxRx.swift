//
//  TxRx.swift
//  SmartBulb
//
//  Created by Mac on 12/21/14.
//  Copyright (c) 2014 GTS. All rights reserved.
//

import UIKit
import Foundation

class TxRx: NSObject {
    var addr:String;
    var port:Int;
    var inputStream: NSInputStream;
    var outputStream: NSOutputStream;
    //var writeByte :UInt8 = 2
    //while inputStream.hasBytesAvailable {
    //inputStream.read(&readByte, maxLength: 1)
    //outputStream.write(&writeByte, maxLength: 2)
    //}
    init(input_addr:String, input_port:Int){
        addr = input_addr;
        port = input_port;
        
        var inp :NSInputStream?
        var out :NSOutputStream?
        
        NSStream.getStreamsToHostWithName(addr, port: port, inputStream: &inp, outputStream: &out)
        
        inputStream = inp!
        outputStream = out!
        
        inputStream.open()
        outputStream.open()
        
    }
    func reinit(){
        var inp :NSInputStream?
        var out :NSOutputStream?
        
        NSStream.getStreamsToHostWithName(addr, port: port, inputStream: &inp, outputStream: &out)
        
        inputStream = inp!
        outputStream = out!
        
        inputStream.open()
        outputStream.open()
    }
    func log_out(){
        inputStream.close();
        outputStream.close();
    }
    
    func start_rx()->String{
        let bufferSize = 1024;
        var buffer = [UInt8](count: bufferSize, repeatedValue: 0)
        
        //while(true){
        if(inputStream.hasBytesAvailable){
            /* Was trying to read byte by byte
            inputStream.read(&readByte, maxLength: 1)
            print("\(Character(UnicodeScalar(readByte))) ", readByte);
            buffer[counter] = readByte;
            counter++;
            if(readByte == 0){
            //print("\(Character(UnicodeScalar(readByte)))");
            //print(NSString(bytes: buffer, length: buffer.count, encoding: NSUTF8StringEncoding));
            print("Get Here!!!");
            buffer = [UInt8](count:100, repeatedValue: 0);
            counter = 0;
            }*/
            let bytesRead = inputStream.read(&buffer, maxLength: bufferSize)
            if bytesRead >= 0 {
                //print(NSString(bytes: &buffer, length: bytesRead, encoding: NSUTF8StringEncoding));
                let message = NSString(bytes: &buffer, length: bytesRead, encoding: NSUTF8StringEncoding) as! String;
                //let index = message.startIndex;
                //if(message[index] !=  ">" && message[index] !=  "*"){
                return message;
                //}
            }
        }
        //}
        return "";
    }

    func login(username:String, password:String)->Bool{
        /*
        var mystring = NSUserDefaults.standardUserDefaults().stringForKey("usernameKey")!;
        var data: NSData = mystring.dataUsingEncoding(NSUTF8StringEncoding)!
        outputStream.write(UnsafePointer<UInt8>(data.bytes), maxLength: data.length)
        mystring = NSUserDefaults.standardUserDefaults().stringForKey("passwordKey")!;
        data = mystring.dataUsingEncoding(NSUTF8StringEncoding)!
        outputStream.write(UnsafePointer<UInt8>(data.bytes), maxLength: data.length)
        sleep(1);
        */
        var login:Bool = false;
        
        var mystring = username;
        var data: NSData = mystring.dataUsingEncoding(NSUTF8StringEncoding)!
        outputStream.write(UnsafePointer<UInt8>(data.bytes), maxLength: data.length)
        mystring = password;
        data = mystring.dataUsingEncoding(NSUTF8StringEncoding)!
        outputStream.write(UnsafePointer<UInt8>(data.bytes), maxLength: data.length)
        sleep(1);
        if(self.start_rx() == "Username:Password:Welcome!"){
            login = true;
        }
        return login;
    }
    
    func send(commend:String){
        let data = commend.dataUsingEncoding(NSUTF8StringEncoding)!
        outputStream.write(UnsafePointer<UInt8>(data.bytes), maxLength: data.length)
    }
}