// pages/login/login.js
Page({
  /**
   * 页面的初始数据
   */
  data: {
    userName:"",
    deviceId:"",
    apiKey:""
  },

  getUserName:function(e){
    this.setData({
      userName:e.detail
    })
  },

  getDeviceId:function(e){
    this.setData({
      deviceId:e.detail
    })
  },

  getApiKey:function(e){
    this.setData({
      apiKey:e.detail
    })
  },

  relaunchController:function(){
    try{
      wx.setStorageSync('userName', this.data.userName);
      wx.setStorageSync('deviceId', this.data.deviceId);
      wx.setStorageSync('apiKey', this.data.apiKey);
    }catch(e) 
    {}
    if(("" != this.data.userName) && ("" != this.data.deviceId) && ("" != this.data.apiKey)){
      wx.reLaunch({
        url: '/pages/controller/controller'
      })
    }else{
    }
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

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
    wx.hideHomeButton();
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