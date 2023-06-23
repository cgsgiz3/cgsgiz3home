function main() {
  document.getElementById("register").onclick = async function () {
    try {
      const res = await fetch("/api/auth/register", {
        method: "POST",
        body: JSON.stringify({
          Login: document.getElementById("login").value,
          Password: document.getElementById("password").value,
        }),
        headers: { "Content-Type": "application/json" },
      });
      const data = await res.json();
      if (res.status === 400 || res.status === 401) {
        document.getElementById("clienterror").innerHTML = `${data.message}. ${
          data.error ? data.error : ""
        }`;
      } else {
        location.assign("/basic");
      }
    } catch (err) {
      console.log(err.message);
    }
  };
  document.getElementById("entrance").onclick = async function () {
    try {
      const res = await fetch("/api/auth/login", {
        method: "POST",
        body: JSON.stringify({
          Login: document.getElementById("login").value,
          Password: document.getElementById("password").value,
        }),
        headers: { "Content-Type": "application/json" },
      });
      const data = await res.json();
      if (res.status === 400 || res.status === 401) {
        document.getElementById("clienterror").innerHTML = `${data.message}. ${
          data.error ? data.error : ""
        }`;
      }
      location.assign("/basic");
    } catch (err) {
      console.log(err.message);
    }
  };
}
window.addEventListener("load", (event) => {
  main();
});
