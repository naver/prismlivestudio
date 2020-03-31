      if (navigator.onLine) {
        var divView = document.getElementById("divNetwork");
        if (divView) {
          document.getElementById("divNetwork").style.display = "none";
        }
      } else if (!window.location.href.startsWith("file")) {
        var divView = document.getElementById("divNetwork")
        if (!divView) {
          divView = document.createElement('div');
          var netLabel = document.createElement('p');
          netLabel.id = "testNetworkP";

          var button = document.createElement("input");
          button.type = "button";
          button.onclick = function () {
            window.location.reload(true);
          };

          if (window.navigator.language.match("ko")) {
            netLabel.innerText = "네트워크 오류가 발생했습니다. 다시 시도해주세요.";
            button.value = "재시도";
            netLabel.style.fontFamily = "'Malgun Gothic','Segoe UI','Dotum','Gulim'";
            button.style.fontFamily = "'Malgun Gothic','Segoe UI','Dotum','Gulim'";
          } else {
            netLabel.innerText = "A network error has occurred. Please try again.";
            button.value = "Retry";
            netLabel.style.fontFamily = "'Segoe UI','Malgun Gothic','Dotum','Gulim'";
            button.style.fontFamily = "'Segoe UI','Malgun Gothic','Dotum','Gulim'";
          }
          document.body.appendChild(divView);
          divView.appendChild(netLabel);
          divView.appendChild(button);

          divView.style = "position: absolute; top: 50%; height: 200px; left: 5% ;width: 90%;  text-align:center; margin-top:-60px";
        }
        divView.style.display = "block";
      }
