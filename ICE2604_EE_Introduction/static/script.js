

maxnum = 1121

// Enter搜索
var focus = document.getElementById("inputtext");
focus.onkeypress = function(event){
    if(event.which === 13){ 
        document.getElementById("search").click();
    }
}

function search() {
    location.href = `search?q=${document.querySelector(".inputbox").value}`
}
function luck() {
    location.href = `${Math.round(Math.random()*maxnum)}`
}
function toggleFav() {
    like = like ^ 1;
    $('.star0').toggleClass('none')
    $('.star1').toggleClass('none')
    $.get('/update',{
        id: id,
        like: like
    })
}