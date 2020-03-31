// 降低成本到指定位置需要降低的仓位
function changeCostRate (income, now, target) {
  return income / ((now - target) / now)
}

console.log(changeCostRate(1154, 62.3, 58))
