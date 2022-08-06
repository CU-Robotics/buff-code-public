use rosrust::ros_info;
use rosrust_msg::{std_msgs, std_msgs::Float64MultiArray};
use std::time::Duration;
use std::{
    sync::{Arc, RwLock},
    thread::sleep,
    time::Instant,
};

pub struct RemoteInput {
    connected: bool,
    input_mode: u8,
    input: Arc<RwLock<[u8; 18]>>,
    dr16_timestamp: Arc<RwLock<Instant>>,

    channels: [u16; 4],
    remote_wheel: u16,

    s1: u8,
    s2: u8,

    mouse: [u16; 3],
    mouse_left: bool,
    mouse_right: bool,

    keyboard: [bool; 16],
    publisher: rosrust::Publisher<Float64MultiArray>,
    subscriber: rosrust::Subscriber,
}

impl RemoteInput {
    pub fn new() -> RemoteInput {
        let input_buffer = Arc::new(RwLock::new([0u8; 18]));
        let timestamp = Arc::new(RwLock::new(Instant::now()));

        let remote_id = "/buffbot/TOPICS/DR16".to_string();
        let remote_topic = rosrust::param(&remote_id).unwrap().get::<String>().unwrap();

        let buffer = Arc::clone(&input_buffer);
        let ts_clone = Arc::clone(&timestamp);

        let sub = rosrust::subscribe(&remote_topic, 5, move |msg: Float64MultiArray| {
            let mut buf = buffer.write().unwrap();
            for (i, c) in msg.data.iter().enumerate() {
                buf[i] = *c as u8;
            }
            let mut ts = ts_clone.write().unwrap();
            *ts = Instant::now();
        });

        match &sub {
            Ok(s) => {}
            _ => {
                ros_info!("subscriber bad");
            }
        }

        let topic_param = "/buffbot/TOPICS/REMOTE_CONTROL".to_string();
        let topic = rosrust::param(&topic_param)
            .unwrap()
            .get::<String>()
            .unwrap();
        let publish = rosrust::publish::<Float64MultiArray>(&topic, 1).unwrap();

        RemoteInput {
            connected: false,
            input_mode: 0,
            input: input_buffer,
            channels: [0u16; 4],
            remote_wheel: 0,

            s1: 0,
            s2: 0,

            mouse: [0u16; 3],
            mouse_left: false,
            mouse_right: false,

            keyboard: [false; 16],
            dr16_timestamp: timestamp,

            publisher: publish,
            subscriber: sub.unwrap(),
        }
    }

    pub fn parse_buffer(&mut self) {
        let buf = self.input.read().unwrap();
        self.channels[0] = ((buf[1] as u16 & 0x07) << 8) | buf[0] as u16;
        self.channels[1] = ((buf[2] as u16 & 0xFC) << 5) | ((buf[1] as u16 & 0xF8) >> 3);
        self.channels[2] =
            (((buf[4] as u16 & 0x01) << 10) | ((buf[3] as u16) << 2)) | (buf[2] as u16 & 0x03);
        self.channels[3] = ((buf[5] as u16 & 0x0F) << 7) | (buf[4] as u16 & 0xFE);

        // ros_info!("{:?}", self.channels);

        self.s1 = (buf[5] & 0x30) >> 4;
        self.s2 = (buf[5] & 0xC0) >> 6;

        // ros_info!("{} {}", self.s1, self.s2);

        self.keyboard[0] = (buf[14] & 0x01) > 0;
        self.keyboard[1] = (buf[14] & 0x02) > 0;
        self.keyboard[2] = (buf[14] & 0x04) > 0;
        self.keyboard[3] = (buf[14] & 0x08) > 0;
        self.keyboard[4] = (buf[14] & 0x10) > 0;
        self.keyboard[5] = (buf[14] & 0x20) > 0;
        self.keyboard[6] = (buf[14] & 0x40) > 0;
        self.keyboard[7] = (buf[14] & 0x80) > 0;

        //second byte of keyboard
        self.keyboard[8] = (buf[15] & 0x01) > 0;
        self.keyboard[9] = (buf[15] & 0x02) > 0;
        self.keyboard[10] = (buf[15] & 0x04) > 0;
        self.keyboard[11] = (buf[15] & 0x08) > 0;
        self.keyboard[12] = (buf[15] & 0x10) > 0;
        self.keyboard[13] = (buf[15] & 0x20) > 0;
        self.keyboard[14] = (buf[15] & 0x40) > 0;
        self.keyboard[15] = (buf[15] & 0x80) > 0;

        // ros_info!("{:?}", self.keyboard);

        // //mouse
        self.mouse[0] = u16::from_be_bytes([buf[7], buf[6]]);
        self.mouse[1] = u16::from_be_bytes([buf[9], buf[8]]);
        self.mouse[2] = u16::from_be_bytes([buf[11], buf[10]]);

        self.mouse_left = buf[12] > 0;
        self.mouse_right = buf[13] > 0;

        // //remote wheel
        self.remote_wheel = ((buf[17] as u16) << 8) | buf[16] as u16;
    }

    pub fn broadcast_joysticks(&self) {
        // 0 - RY - 364:1684
        // 1 - RX - 268:1684
        // 2 - LY - 364:1684
        // 3 - LX - 476:1704

        let xvel: f64 = (self.channels[3] as f64 - 1024.0) / ((1704.0 - 476.0) / 2.0);
        let yvel: f64 = (self.channels[2] as f64 - 1024.0) / ((1684.0 - 364.0) / 2.0);

        let rotv: f64 = match self.s1 {
            1 => 1.0,
            2 => -1.0,
            3 => 0.0,
            _ => 0.0,
        };

        let gphi: f64 = (self.channels[1] as f64 - 1024.0) / ((1684.0 - 268.0) / 2.0);
        let gpsi: f64 = (self.channels[0] as f64 - 1024.0) / ((1684.0 - 364.0) / 2.0);

        let mut msg = Float64MultiArray::default();
        msg.data = vec![xvel, yvel, rotv, gphi, gpsi];
        let _result = self.publisher.send(msg).unwrap();
    }

    pub fn broadcast_keyboard(&self) {}

    pub fn broadcast_control(&self) {
        // ctrl_msg = [inputType,xdot,ydot,psidotC,phidotT,psidotT]
        match self.input_mode {
            0 => self.broadcast_joysticks(),
            1 => self.broadcast_keyboard(),
            _ => println!("Unknown RC input mode {}", self.input_mode),
        }
    }

    pub fn spin(&mut self) {
        let param_id = format!("/buffbot/HID_ACTIVE");

        while rosrust::is_ok() {
            let time_stamp = Instant::now();

            if rosrust::param(&param_id).unwrap().exists().unwrap() {
                if rosrust::param(&param_id).unwrap().get::<i32>().unwrap() > 0 {
                    self.parse_buffer();
                }
            }

            self.broadcast_control();

            if (time_stamp.elapsed().as_millis() as u64) < 20 {
                sleep(Duration::from_millis(
                    20 - time_stamp.elapsed().as_millis() as u64,
                ));
            }
        }
    }
}