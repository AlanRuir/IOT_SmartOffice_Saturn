// pages/controller/controller.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    userName:"",
    deviceId:"",
    apiKey:"",
    gradientColor:{
      '0%': '#ffd01e',
      '100%': '#ee0a24',
    },
    isOnline:"",
    AirFanSwitch:0,
    AirFanLevel:0,
    Temperature:0,
    Humidity:0,
    LightSwitch:0,
    LightLevel:0,
    Switch1:0,
    Switch2:0,
    Switch3:0
  },

  onAirFanSwitchChange({ detail }) {
    // 需要手动对 checked 状态进行更新
    if(detail){
      this.setData({
        AirFanSwitch:1
      });
    }else{
      this.setData({
        AirFanSwitch:0
      })
    }

    this.controllerCmds("AirFanSwitch", this.data.AirFanSwitch);
  },

  onAirFanLevelChange(event){
    this.setData({
      AirFanLevel:event.detail
    });
    wx.showToast({
      icon: 'none',
      title: `当前值：${event.detail}`
    });

    this.controllerCmds("AirFanLevel", this.data.AirFanLevel * 10);
  },

  onLightSwitchChange({ detail }){
    if(detail){
      this.setData({
        LightSwitch:1
      });
    }else{
      this.setData({
        LightSwitch:0
      })
    }

    this.controllerCmds("LightSwitch", this.data.LightSwitch);
  },

  onLightLevelChange(event){
    this.setData({
      LightLevel:event.detail
    });
    wx.showToast({
      icon: 'none',
      title: `当前值：${event.detail}`
    });

    this.controllerCmds("LightLevel", this.data.LightLevel * 10);
  },

  onSwitch1Change({ detail }){
    if(detail){
      this.setData({
        Switch1:1
      });
    }else{
      this.setData({
        Switch1:0
      });
    }

    this.controllerCmds("Switch1", this.data.Switch1);
  },

  onSwitch2Change({ detail }){
    if(detail){
      this.setData({
        Switch2:1
      });
    }else{
      this.setData({
        Switch2:0
      })
    }

    this.controllerCmds("Switch2", this.data.Switch2);
  },

  onSwitch3Change({ detail }){
    if(detail){
      this.setData({
        Switch3:1
      });
    }else{
      this.setData({
        Switch3:0
      })
    }

    this.controllerCmds("Switch3", this.data.Switch3);
  },

  controllerCmds: function(handwearName, cmdvalue) {
    console.log(handwearName + ":" + cmdvalue);
    wx.request({
      url: 'https://api.heclouds.com/cmds?device_id=' + this.data.deviceId,
      method: 'POST',
      header: {
        'content-type': 'application/x-www-form-urlencoded',
        "api-key": this.data.apiKey
      },
      data:handwearName + ":" + cmdvalue,
      success:result=>{
        console.log("发送命令成功！");
      }
    });
  },

  refreshController: function(){
    var that = this;
    console.log("刷新设备状态");
    wx.request({
      url: 'https://api.heclouds.com/devices/' + that.data.deviceId + '/datastreams',
      header: {
        'content-type': 'application/x-www-form-urlencoded',
        "api-key": that.data.apiKey
      },
      data:{
  
      },
      success:result=>{
        console.log(result);
        this.setData({
          AirFanSwitch:result.data.data[0].current_value,
          AirFanLevel:result.data.data[1].current_value,
          Temperature:result.data.data[2].current_value,
          Humidity:result.data.data[3].current_value,
          LightSwitch:result.data.data[4].current_value,
          LightLevel:result.data.data[8].current_value,
          Switch1:result.data.data[5].current_value,
          Switch2:result.data.data[6].current_value,
          Switch3:result.data.data[7].current_value
        });
      }
    })
  },
  
  getDeviceInfo: function(){
    var that = this;

    wx.request({
      url: 'https://api.heclouds.com/devices/' + that.data.deviceId,
      header: {
        'content-type': 'application/x-www-form-urlencoded',
        "api-key": that.data.apiKey
      },
      data:{
  
      },
      success:result=>{
        if(result.data.data.online){
          this.setData({
            isOnline:"在线"
          });
          that.refreshController();
        }else{
          this.setData({
            isOnline:"离线",
            AirFanSwitch:0,
            AirFanLevel:0,
            Temperature:0,
            Humidity:0,
            LightSwitch:0,
            LightLevel:0,
            Switch1:0,
            Switch2:0,
            Switch3:0
          })
        }
      },
      fail:result=>{
        console.log("设备状态查看请求失败！");
      }
    });
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (e) {
    try {
      var userName = wx.getStorageSync('userName');
      var deviceId = wx.getStorageSync('deviceId');
      var apiKey = wx.getStorageSync('apiKey');
      this.setData({
        userName:userName,
        deviceId:deviceId,
        apiKey:apiKey
      });
    } catch (e) {}
    var that = this;

    that.getDeviceInfo();
    that.data.myintervalid = setInterval(function () {
      that.onShow()
    }, 3000);
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    var that = this;
    wx.hideHomeButton();
    that.getDeviceInfo();
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {
  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})