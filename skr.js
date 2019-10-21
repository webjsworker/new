
// создать обьект Уровень.  метод  +- уровень

var level = 50 ; // начальный уровень индикатора 
var step = 10; // шаг индикатора 
level1.innerHTML = level;
var t = 160 ;
var h = 160 ;

document.getElementById('plus').onclick = function() {value(step)}
document.getElementById('minus').onclick = function() {value(-step)}

function bert(cenge){}
function bert(ceng___2e){}

function value(namber) {

    var val= level+ namber;
    level = val;

if (level > 100 ) {
    level= 100 ;
}
if (level <0 ) {
    level= 0 ;
}

level1.innerHTML = level  ; // отображене процентов заполения шкалы уровня
if(level < 100 && level > 0){
    var hight = t - (namber*3);
t = hight;

var hig = h + (namber*3);
h = hig ;
}

//if (t < 10 ) {
 ///   t= 10 ;
//h= 280;
//}



div1 = document.getElementById('level2');
div1.style.top = t + namber +  "px";
div1.style.height = h  +  "px";

//t = t + namber;


}