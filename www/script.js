const API = "";
async function getState() {
    const r = await fetch(API + "/state");
    return r.json();
}
async function sendAnswer(ans) {
    await fetch(API + "/answer", { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ answer: ans }) });
}
async function newGame() {
    await fetch(API + "/newgame", { method: "POST" });
}

const questionDiv = document.getElementById("question");
const historyDiv = document.getElementById("history");
const form = document.getElementById("form");
const answer = document.getElementById("answer");
const restart = document.getElementById("restart");
const heroCard = document.getElementById("heroCard");
const dragonCard = document.getElementById("dragonCard");
const heroImg = document.getElementById("heroImg");
const dragonImg = document.getElementById("dragonImg");
const dragonName = document.getElementById("dragonName");
const dragonStatus = document.getElementById("dragonStatus");
const heroStatus = document.getElementById("heroStatus");
const celebrationEffect = document.getElementById("celebration");
const defeatEffect = document.getElementById("defeat");

let lastEnemyColor = '';

async function render() {
    const s = await getState();
    questionDiv.textContent = s.question || "";

    // Обновляем историю
    historyDiv.innerHTML = s.history ? s.history.replace(/\n/g, '<br>') : "";

    // Прокручиваем историю вниз
    historyDiv.scrollTop = historyDiv.scrollHeight;

    // Обновляем статус героя
    heroStatus.textContent = `❤️ Здоровье: ${s.hero.hp} | ⚔️ Атака: ${s.hero.atk} | ⭐ Опыт: ${s.hero.exp}`;

    // Обновляем текущего дракона
    if (s.enemy.name !== "-") {
        dragonName.textContent = s.enemy.name;
        dragonStatus.textContent = `❤️ Здоровье: ${s.enemy.hp} | ⚔️ Атака: ${s.enemy.atk}`;

        // Устанавливаем изображение дракона в зависимости от цвета
        if (s.enemy.color !== lastEnemyColor) {
            const dragonImageMap = {
                'Yellow': 'images/yellow-dragon.jpg',
                'Blue': 'images/blue-dragon.jpg',
                'Purple': 'images/purple-dragon.jpg',
                'Red': 'images/red-dragon.jpg',
                'Green': 'images/green-dragon.jpg'
            };
            dragonImg.src = dragonImageMap[s.enemy.color] || 'images/not-yet.jpg';
            lastEnemyColor = s.enemy.color;
        }

        // Показываем карточку дракона
        dragonCard.style.display = 'block';
    } else {
        // Скрываем карточку дракона, если врага нет
        dragonCard.style.display = 'none';
    }

    // Анимации при атаке
    if (s.info && s.info.includes('атакует')) {
        if (s.info.includes('Герой атакует')) {
            animateAttack(heroCard, dragonCard);
        } else if (s.info.includes('атакует героя')) {
            animateAttack(dragonCard, heroCard);
        }
    }

    // Обработка окончания игры
    if (s.finished) {
        answer.disabled = true;
        form.querySelector("button").disabled = true;

        if (s.win) {
            showCelebration();
        } else {
            showDefeat();
        }
    } else {
        answer.disabled = false;
        form.querySelector("button").disabled = false;
        answer.focus();
    }
}

// Анимация атаки
function animateAttack(attacker, target) {
    // Сбрасываем предыдущие анимации
    attacker.classList.remove('attacking');
    target.classList.remove('taking-damage');

    // Принудительное обновление DOM
    void attacker.offsetWidth;
    void target.offsetWidth;

    // Добавляем анимации
    attacker.classList.add('attacking');
    target.classList.add('taking-damage');

    // Убираем анимации после завершения
    setTimeout(() => {
        attacker.classList.remove('attacking');
        target.classList.remove('taking-damage');
    }, 600);
}

// Показать праздничный эффект
function showCelebration() {
    celebrationEffect.innerHTML = '';
    celebrationEffect.style.display = 'block';

    const emojis = ['🎉', '🎊', '⭐', '🏆', '👑', '🎈', '✨'];
    for (let i = 0; i < 50; i++) {
        const emoji = document.createElement('div');
        emoji.className = 'celebration-emoji';
        emoji.textContent = emojis[Math.floor(Math.random() * emojis.length)];
        emoji.style.left = Math.random() * 100 + 'vw';
        emoji.style.animationDelay = Math.random() * 2 + 's';
        emoji.style.fontSize = (Math.random() * 20 + 20) + 'px';
        celebrationEffect.appendChild(emoji);
    }

    setTimeout(() => {
        celebrationEffect.style.display = 'none';
        celebrationEffect.innerHTML = '';
    }, 3000);
}

// Показать эффект поражения
function showDefeat() {
    defeatEffect.innerHTML = '';
    defeatEffect.style.display = 'block';

    const emojis = ['💀', '😵', '⚰️', '🪦', '☠️'];
    for (let i = 0; i < 30; i++) {
        const emoji = document.createElement('div');
        emoji.className = 'defeat-emoji';
        emoji.textContent = emojis[Math.floor(Math.random() * emojis.length)];
        emoji.style.left = Math.random() * 100 + 'vw';
        emoji.style.animationDelay = Math.random() * 2 + 's';
        emoji.style.fontSize = (Math.random() * 15 + 15) + 'px';
        defeatEffect.appendChild(emoji);
    }

    setTimeout(() => {
        defeatEffect.style.display = 'none';
        defeatEffect.innerHTML = '';
    }, 3000);
}

form.addEventListener("submit", async (e) => {
    e.preventDefault();
    const val = answer.value.trim();
    if (!val) return;
    await sendAnswer(val);
    answer.value = "";
    await render();
});

restart.addEventListener("click", async () => {
    await newGame();
    celebrationEffect.style.display = 'none';
    defeatEffect.style.display = 'none';
    await render();
});

// Запускаем рендер при загрузке
render();