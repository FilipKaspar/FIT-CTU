smazatBtn = document.getElementById('employee_Smazat');
if(smazatBtn) {
    smazatBtn.onclick = function (event) {
        var result = confirm("Are you sure you want to delete this user?");

        if (!result) {
            event.preventDefault();
        }
    };
}