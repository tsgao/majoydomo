//
//  ChattingViewController.swift
//  SmartBulb
//
//  Created by Mac on 12/1/15.
//  Copyright © 2015 GTS. All rights reserved.
//

import UIKit
import AVFoundation

class ChattingViewController: UIViewController, UITextViewDelegate {
    
    let deligate = UIApplication.sharedApplication().delegate as! AppDelegate
    
    @IBOutlet weak var send_button: UIButton!
    @IBOutlet weak var chatting_window: UITextView!
    //@IBOutlet weak var message_send: UITextField!
    @IBOutlet weak var input: UITextView!
    
    override func viewDidLoad() {
        
        prounce("Welcome to chatting room, please type your message in the left bottom white box")
        
        super.viewDidLoad()
        //self.message_send.delegate = self;
        self.input.delegate = self;
        // Do any additional setup after loading the view, typically from a nib.
        let receive_message = NSThread(target:self, selector:"update_message", object:nil)
        receive_message.start();
        
        //deligate.txrx?.login();
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    func update_message() {
        //var utt = AVSpeechUtterance(string: "")
        //utt.voice = AVSpeechSynthesisVoice(language: "en-US")
        //utt.rate = 0.4
        //let syn = AVSpeechSynthesizer()
        
        while(true){
            if(view.window != nil){
                let message = deligate.txrx!.start_rx();
                let index = message.startIndex;
                //if(message[index] !=  ">" && message[index] !=  "*"){
                if(message != "" && message[index] != "U" && message[index] != "P" && message[index] != ">" && message[index] != "*"){
                    dispatch_async(dispatch_get_main_queue()) {
                        self.chatting_window.text?.appendContentsOf(message);
                        self.chatting_window.text?.appendContentsOf("\n");
                    }
                    prounce(message)
                    //utt = AVSpeechUtterance(string: message)
                    //syn.speakUtterance(utt)
                }
                NSThread .sleepForTimeInterval(0.5);
            }
        }
    }
    
    
    @IBAction func send_message(sender: AnyObject) {
        if input.text != "" {
            //deligate.txrx!.send(">" + message_send.text!);
            deligate.txrx!.send(">" + input.text!);
            clear_input();
        }
    }
    
    func prounce (words: String){
        let utt = AVSpeechUtterance(string: words)
        utt.voice = AVSpeechSynthesisVoice(language: "en-US")
        utt.rate = 0.45
        let syn = AVSpeechSynthesizer()
        syn.speakUtterance(utt)
    }
    
    func textFieldShouldReturn(textField: UITextField) -> Bool {
        if input.text != "" {
            //self.view.endEditing(true)
            //deligate.txrx!.send(">" + message_send.text!);
            deligate.txrx!.send(">" + input.text!);
            clear_input();
        }
        return false
    }
    
    func clear_input(){
        //message_send.text = ""
        input.text = ""
    }
}