//
//  TxRx.swift
//  SmartBulb
//
//  Created by Mac on 12/21/14.
//  Copyright (c) 2014 GTS. All rights reserved.
//

import UIKit
import Foundation

class TxRx {
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
    
    func start_rx(){
        var readByte :UInt8 = 0
        
        while(true){
            if(inputStream.hasBytesAvailable){
                inputStream.read(&readByte, maxLength: 1)
                print("\(Character(UnicodeScalar(readByte)))");
            }
        }
    }
    func login(){
        var mystring = "tsgao";
        var data: NSData = mystring.dataUsingEncoding(NSUTF8StringEncoding)!
        outputStream.write(UnsafePointer<UInt8>(data.bytes), maxLength: data.length)
        mystring = "password";
        data = mystring.dataUsingEncoding(NSUTF8StringEncoding)!
        outputStream.write(UnsafePointer<UInt8>(data.bytes), maxLength: data.length)
        sleep(1);
    }
    
    func send(commend:String){
        var data = commend.dataUsingEncoding(NSUTF8StringEncoding)!
        outputStream.write(UnsafePointer<UInt8>(data.bytes), maxLength: data.length)
    }
}