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
const heroHealthFill = document.getElementById("heroHealthFill");
const enemyHealthFill = document.getElementById("enemyHealthFill");
const heroHpValue = document.getElementById("heroHpValue");
const enemyHpValue = document.getElementById("enemyHpValue");
const heroAtk = document.getElementById("heroAtk");
const heroExp = document.getElementById("heroExp");
const enemyAtk = document.getElementById("enemyAtk");
const celebrationEffect = document.getElementById("celebration");
const defeatEffect = document.getElementById("defeat");

let lastEnemyColor = '';

// Анимация атаки
// Функция создания партиклов атаки
function createAttackParticles(attacker, target, type) {
    const attackerRect = attacker.getBoundingClientRect();
    const targetRect = target.getBoundingClientRect();

    const attackEmojis = type === 'hero' ?
        ['⚔️', '✨', '🌟', '💫', '🔥'] :
        ['🔥', '💀', '☠️', '⚡', '🌪️'];

    const particleColor = type === 'hero' ? '#3b82f6' : '#ef4444';

    // Партиклы летящие от атакующего к цели
    for (let i = 0; i < 12; i++) {
        const particle = document.createElement('div');
        particle.className = 'particle attack-particle';
        particle.textContent = attackEmojis[Math.floor(Math.random() * attackEmojis.length)];
        particle.style.color = particleColor;
        particle.style.fontSize = (Math.random() * 16 + 14) + 'px';

        // Начальная позиция от атакующего
        const startX = attackerRect.left + attackerRect.width / 2;
        const startY = attackerRect.top + attackerRect.height / 2;

        // Конечная позиция у цели
        const endX = targetRect.left + targetRect.width / 2;
        const endY = targetRect.top + targetRect.height / 2;

        particle.style.left = startX + 'px';
        particle.style.top = startY + 'px';

        const translateX = endX - startX;
        const translateY = endY - startY;

        particle.style.setProperty('--translate-x', translateX + 'px');
        particle.style.setProperty('--translate-y', translateY + 'px');

        document.body.appendChild(particle);

        setTimeout(() => {
            if (particle.parentNode) {
                particle.parentNode.removeChild(particle);
            }
        }, 800);
    }
}

// Функция создания партиклов урона
function createDamageParticles(target, type) {
    const rect = target.getBoundingClientRect();
    const damageEmojis = type === 'hero' ?
        ['💔', '🩸', '😵', '💢'] :
        ['💥', '🔥', '⚡', '💢', '❌'];

    const particleColor = type === 'hero' ? '#ef4444' : '#f59e0b';

    for (let i = 0; i < 8; i++) {
        const particle = document.createElement('div');
        particle.className = 'particle damage-particle';
        particle.textContent = damageEmojis[Math.floor(Math.random() * damageEmojis.length)];
        particle.style.color = particleColor;
        particle.style.fontSize = (Math.random() * 16 + 14) + 'px';

        // Случайная позиция внутри карточки
        const posX = rect.left + Math.random() * rect.width;
        const posY = rect.top + Math.random() * rect.height;

        particle.style.left = posX + 'px';
        particle.style.top = posY + 'px';

        // Случайное направление разлета
        const angle = Math.random() * Math.PI * 2;
        const distance = Math.random() * 60 + 30;
        const translateX = Math.cos(angle) * distance;
        const translateY = Math.sin(angle) * distance;

        particle.style.setProperty('--translate-x', translateX + 'px');
        particle.style.setProperty('--translate-y', translateY + 'px');

        document.body.appendChild(particle);

        setTimeout(() => {
            if (particle.parentNode) {
                particle.parentNode.removeChild(particle);
            }
        }, 1000);
    }
}

// Обновленная функция анимации атаки
// Обновленная функция анимации атаки
function animateAttack(attacker, target, type) {
    attacker.classList.remove('attacking');
    target.classList.remove('taking-damage');
    target.classList.remove('damage-glow');

    void attacker.offsetWidth;
    void target.offsetWidth;

    attacker.classList.add('attacking');
    target.classList.add('taking-damage');
    target.classList.add('damage-glow');

    // Создаем партиклы атаки
    createAttackParticles(attacker, target, type);

    // Создаем партиклы урона на цели
    setTimeout(() => {
        createDamageParticles(target, type === 'hero' ? 'enemy' : 'hero');

        // Проверяем, умер ли враг после атаки героя
        if (type === 'hero') {
            const currentEnemy = dragonCard.querySelector('.character-info h4').textContent;
            if (currentEnemy !== "-" && !dragonCard.classList.contains('dragon-dying')) {
                // Проверяем HP врага через состояние игры
                checkEnemyDeath();
            }
        }
    }, 300);

    setTimeout(() => {
        attacker.classList.remove('attacking');
        target.classList.remove('taking-damage');
        target.classList.remove('damage-glow');
    }, 600);
}

// Функция проверки смерти врага
async function checkEnemyDeath() {
    const s = await getState();

    if (s.enemy.hp <= 0 && s.enemy.name !== "-") {
        // Запускаем анимацию смерти дракона
        animateDragonDeath().then(() => {
            // После завершения анимации смерти, показываем нового врага
            setTimeout(() => {
                animateDragonSpawn();
            }, 200);
        });
    }
}

// Анимация смерти дракона
function animateDragonDeath() {
    return new Promise((resolve) => {
        dragonCard.classList.add('dragon-dying');

        setTimeout(() => {
            dragonCard.classList.remove('dragon-dying');
            resolve();
        }, 1000);
    });
}

// Анимация появления нового дракона
function animateDragonSpawn() {
    dragonCard.classList.add('dragon-spawning');

    setTimeout(() => {
        dragonCard.classList.remove('dragon-spawning');
    }, 1000);
}

// Обновите функцию render для обработки появления нового врага
async function render() {
    const s = await getState();
    questionDiv.textContent = s.question || "";

    // Обновляем историю
    historyDiv.innerHTML = s.history ? s.history.replace(/\n/g, '<br>') : "";
    historyDiv.scrollTop = historyDiv.scrollHeight;

    // Обновляем героя
    const heroMaxHp = 35;
    const heroHpPercent = (s.hero.hp / heroMaxHp) * 100;
    heroHealthFill.style.width = `${heroHpPercent}%`;
    heroHpValue.textContent = `${s.hero.hp}/${heroMaxHp}`;
    heroAtk.textContent = s.hero.atk;
    heroExp.textContent = s.hero.exp;

    // Обновляем врага
    if (s.enemy.name !== "-") {
        // Если враг появился, и у него полное HP, запускаем анимацию появления
        const enemyMaxHp = s.enemy.name.includes('Dragon') ? 15 : 12;
        if (s.enemy.hp === enemyMaxHp && !dragonCard.classList.contains('dragon-spawning')) {
            animateDragonSpawn();
        }

        dragonName.textContent = s.enemy.name;

        // Определяем максимальное HP врага
        const enemyHpPercent = (s.enemy.hp / enemyMaxHp) * 100;
        enemyHealthFill.style.width = `${enemyHpPercent}%`;
        enemyHpValue.textContent = `${s.enemy.hp}/${enemyMaxHp}`;
        enemyAtk.textContent = s.enemy.atk;

        // Устанавливаем изображение врага
        if (s.enemy.color !== lastEnemyColor) {
            const enemyImageMap = {
                'Yellow': 'images/yellow-dragon.jpg',
                'Blue': 'images/blue-dragon.jpg',
                'Purple': 'images/purple-dragon.jpg',
                'Red': 'images/red-dragon.jpg',
                'Green': 'images/green-dragon.jpg'
            };

            if (s.enemy.name.includes('Goblin')) {
                dragonImg.src = 'images/goblin.jpg';
            } else {
                dragonImg.src = enemyImageMap[s.enemy.color] || 'images/not-yet.jpg';
            }
            lastEnemyColor = s.enemy.color;
        }

        dragonCard.style.display = 'block';
    } else {
        dragonCard.style.display = 'none';
        enemyHealthFill.style.width = '0%';
        enemyHpValue.textContent = '0/0';
    }

    // Анимации при атаке
    if (s.info && s.info.includes('атакует')) {
        if (s.info.includes('атакует героя')) {
            // Враг атакует героя
            animateAttack(dragonCard, heroCard, 'enemy');
        } else {
            // Герой атакует врага
            animateAttack(heroCard, dragonCard, 'hero');
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
// Эффект победы
function showCelebration() {
    celebrationEffect.style.display = 'block';
    const emojis = ['🎉', '🎊', '⭐', '🏆', '👑'];

    for (let i = 0; i < 30; i++) {
        const emoji = document.createElement('div');
        emoji.className = 'celebration-emoji';
        emoji.textContent = emojis[Math.floor(Math.random() * emojis.length)];
        emoji.style.left = Math.random() * 100 + 'vw';
        emoji.style.animationDelay = Math.random() * 1 + 's';
        celebrationEffect.appendChild(emoji);

        setTimeout(() => {
            if (emoji.parentNode) {
                emoji.parentNode.removeChild(emoji);
            }
        }, 2000);
    }

    setTimeout(() => {
        celebrationEffect.style.display = 'none';
        celebrationEffect.innerHTML = '';
    }, 2000);
}

// Эффект поражения
function showDefeat() {
    defeatEffect.style.display = 'block';
    const emojis = ['💀', '😵', '💔', '⚰️'];

    for (let i = 0; i < 20; i++) {
        const emoji = document.createElement('div');
        emoji.className = 'defeat-emoji';
        emoji.textContent = emojis[Math.floor(Math.random() * emojis.length)];
        emoji.style.left = Math.random() * 100 + 'vw';
        emoji.style.animationDelay = Math.random() * 0.5 + 's';
        defeatEffect.appendChild(emoji);

        setTimeout(() => {
            if (emoji.parentNode) {
                emoji.parentNode.removeChild(emoji);
            }
        }, 2000);
    }

    setTimeout(() => {
        defeatEffect.style.display = 'none';
        defeatEffect.innerHTML = '';
    }, 2000);
}

// Обработчики событий
form.addEventListener("submit", async (e) => {
    e.preventDefault();
    const ans = answer.value.trim();
    if (!ans) return;

    await sendAnswer(ans);
    answer.value = "";
    await render();
});

restart.addEventListener("click", async () => {
    await newGame();

    // Сброс анимационных классов
    dragonCard.classList.remove('dragon-dying', 'dragon-spawning');

    await render();
    celebrationEffect.style.display = 'none';
    defeatEffect.style.display = 'none';
    celebrationEffect.innerHTML = '';
    defeatEffect.innerHTML = '';
});
// Запуск игры
render();
