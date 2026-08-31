# แนวทางการเขียน Section 4.1 (ผลการทดลอง Culture-Performance, ข้อ 3.3)

---

## ส่วนที่ 1 — โครงสร้างที่แนะนำ

หลักการ: **Results เดินตามลำดับเดียวกับที่ Methods 3.3 วางไว้** คือ คุณภาพน้ำ → อัตรารอด → การเจริญเติบโต → ประสิทธิภาพอาหาร → ผลผลิตรวม เพราะ 3.3 อธิบายตัวแปรตามลำดับนี้ ผู้อ่านจะตามได้โดยไม่ต้องย้อนกลับ และเป็นลำดับเชิงเหตุผล (น้ำดี → ตายน้อย → โตดี → กินอาหารคุ้ม → ผลผลิตสูง)

ตั้งหัวข้อเป็น **4.1. Culture Performance** แล้วแบ่งย่อหน้า (ไม่ต้องทำหัวข้อย่อย 4.1.1 — MDPI นิยมย่อหน้าที่มีประโยคนำชัดเจนมากกว่า) ดังนี้

| ย่อหน้า | เนื้อหา | อ้างอิงภาพ/ตาราง |
|---|---|---|
| ¶1 | ประโยคนำ + บริบทการทดลอง (70 วัน, สุ่มวัด wk 0–9, n=30/บ่อ/ครั้ง) และ**ประกาศข้อจำกัดทางสถิติทันที** (n=1 ต่อกลุ่ม → descriptive เท่านั้น) | — |
| ¶2 | คุณภาพน้ำรายวัน (DO, pH, อุณหภูมิ) — ค่าเฉลี่ย±SD, พิสัย, จำนวนวันที่หลุดเกณฑ์ | Table 2 |
| ¶3 | อัตรารอด — เส้นทางการตายรายสัปดาห์ + ค่าปลายทาง | Figure 10c, Table 3 |
| ¶4 | การเจริญเติบโต — น้ำหนัก/ความยาว/SGR/ADG | Figure 10a,b, Table 3 |
| ¶5 | ประสิทธิภาพอาหาร — อาหารที่กินจริง, เศษเหลือ, FCR | Table 3 |
| ¶6 | ผลผลิตรวม (biomass, g m⁻³) + ประโยคสรุปเชื่อมไป Discussion | Table 3 |

**กฎเหล็ก 5 ข้อสำหรับหัวข้อนี้**

1. **ห้ามใช้คำว่า “significant / significantly”** เด็ดขาด เพราะ n = 1 cage ต่อกลุ่ม (3.3 ระบุเองแล้ว) ให้ใช้ *higher, lower, exceeded, by a margin of* แทน และห้ามใส่ p-value
2. **SD ที่รายงานคือ SD ของกุ้ง 30 ตัวที่วัด ไม่ใช่ SD ของ treatment** — ต้องเขียนกำกับว่า “(mean ± SD of the 30 prawns measured)” ทุกครั้ง มิฉะนั้น reviewer จะเข้าใจผิดว่าเป็น replicate
3. **แยก “ผล” ออกจาก “การตีความ”** — 4.1 บอกว่าเกิดอะไรขึ้น ส่วน “เพราะ DO เสถียรกว่าจึงรอดสูงกว่า” เก็บไว้ที่ Discussion (หรือย่อหน้าท้ายสุดของ 4 ถ้าเป็นรูปแบบ Results and Discussion รวมกัน)
4. **ตัวเลขในเนื้อความต้องไม่ซ้ำตาราง 100%** — เนื้อความหยิบเฉพาะตัวเลขชี้ขาด (ค่าปลายทาง + ส่วนต่าง) ที่เหลือให้ตารางพูด
5. **ทศนิยมให้คงที่**: อัตรารอด/เปอร์เซ็นต์ 1 ตำแหน่ง, น้ำหนัก 2–3 ตำแหน่ง, ความยาว 2 ตำแหน่ง, FCR 2 ตำแหน่ง, DO/pH/อุณหภูมิ 1–2 ตำแหน่ง

**เลขที่ภาพและตาราง** — บทความใช้ Figure 1–9 และ Table 1 ไปแล้ว ดังนั้น 4.1 ต้องเริ่มที่ **Table 2, Table 3 และ Figure 10**

---

## ส่วนที่ 2 — ประเด็นที่ต้องตัดสินใจก่อนส่ง (พบจากการตรวจข้อมูลดิบ)

> ⚠️ **1. วันที่ 63 กับวันที่ 70 ไม่ตรงกัน** — ชีต Weekly Sampling วัดการเจริญเติบโตถึง Week 9 = **วันที่ 63** เท่านั้น แต่ Daily Log บันทึกการตายถึง **วันที่ 70** (อัตรารอดที่วันจับ = 66.0% Control, 78.0% Treatment ซึ่งต่างจาก 68.8/79.5% ที่วันที่ 63)
> **ทางเลือก (ก)** เขียนให้ชัดว่าค่าการเจริญเติบโตคือ “at the final weekly sampling (day 63)” และรายงานอัตรารอดที่วันจับแยกอีกหนึ่งประโยค — ร่างด้านล่างใช้วิธีนี้
> **ทางเลือก (ข)** ถ้ามีข้อมูลชั่งน้ำหนักตอนจับจริงวันที่ 70 ให้เพิ่มแถว “Harvest (day 70)” ในตาราง จะสมบูรณ์กว่ามาก และควรแก้ 3.3 ให้ระบุว่ามีการสุ่มวัดที่วันจับด้วย

> ⚠️ **2. DO รายวัน (อ่านเวลา 12:00) ไม่เคยต่ำกว่า 4.0 mg/L ในทั้งสองบ่อ** — ต่ำสุดคือ 4.2 mg/L (Control, วันที่ 66) ดังนั้น**ห้ามอ้างเหตุการณ์ DO วิกฤตใน 4.1** เพราะ event เหล่านั้นมาจากข้อมูล 15 นาทีของการทดลอง 3.5 ให้ 4.1 พูดเฉพาะสถิติรายวัน แล้วอ้างต่อว่า “see Section 4.3” จะไม่ขัดกันเอง

> ⚠️ **3. Control ได้อาหารรวมน้อยกว่า (2,776 vs 3,186 g)** เพราะสูตรให้อาหารคำนวณจากชีวมวลที่ต่ำกว่า ไม่ใช่เพราะให้อาหารต่างกัน — **ต้องเขียนอธิบายจุดนี้** ไม่งั้น reviewer จะสงสัยว่า FCR ที่แย่กว่าเกิดจากการให้อาหารเกิน ร่างด้านล่างใส่ประโยคนี้ไว้แล้ว

> ⚠️ **4. อุณหภูมิน้ำลดลงในสัปดาห์ที่ 8–9** (จาก ~29 °C เหลือ ~26 °C) เท่ากันทั้งสองบ่อ — ควรระบุว่าเป็นการเปลี่ยนแปลงตามฤดูกาลที่กระทบทั้งสองบ่อเท่ากัน จึงไม่รบกวนการเปรียบเทียบ

> ⚠️ **5. อัตราการเจริญเติบโตของทั้งสองกลุ่มต่ำกว่าที่คาด** (0.62 → 1.39–1.57 g ใน 63 วัน) ถ้าเป็นค่าปกติของ *M. lanchesteri* ให้อ้างอิงวรรณกรรมกำกับใน Discussion เพื่อกันคำถาม

---

## ส่วนที่ 3 — ร่างข้อความ 4.1 (ภาษาอังกฤษ พร้อมวางลงบทความ)

### 4.1. Culture Performance

Over the 70-day grow-out, the two cages were monitored daily and sampled at ten weekly time points (weeks 0–9, days 0–63), with 30 prawns individually weighed and measured in each cage at every sampling. Because the trial used a single cage per group, the results below are reported descriptively; the standard deviations quoted for body weight and total length are those of the 30 prawns measured at each sampling and characterize within-cage variation among individuals, not variation among replicate cages. No inferential test was applied, and the differences between the two cages are therefore described in magnitude rather than in terms of statistical significance.

Daily water-quality records (Table 2) show that both cages remained within the normal operating band defined in Section 3.2 throughout the trial, but that the two differed in stability. Mean dissolved oxygen was 6.32 ± 0.33 mg L⁻¹ in the Treatment cage and 5.76 ± 0.56 mg L⁻¹ in the Control, and the coefficient of variation of the DO series was correspondingly lower in the Treatment cage (5.2% versus 9.7%). The daily minimum reached 5.7 mg L⁻¹ in the Treatment cage but fell to 4.2 mg L⁻¹ in the Control, and DO in the Control entered the surveillance band (< 5.0 mg L⁻¹) on five days (days 51, 57, 58, 66, and 67), whereas the Treatment cage did so on none. The same pattern appeared in pH, which spanned 1.1 units in the Control (6.9–8.0) but only 0.5 units in the Treatment cage (7.4–7.9). Water temperature, by contrast, was effectively identical in the two cages (28.31 ± 1.11 °C versus 28.24 ± 1.12 °C), as expected of two units standing side by side under the same roof; the decline of about 3 °C over weeks 8–9 reflects seasonal cooling and affected both cages equally. Neither cage recorded a value outside the critical thresholds for pH or temperature at the daily reading time. The sub-daily dissolved-oxygen dynamics, including the depletion events that triggered automatic aeration, are resolved at the 15 min logging interval and are reported separately in Section 4.3.

Survival diverged progressively (Figure 10c). Both cages lost prawns most rapidly during the first week, as is usual after stocking, with 103 and 89 mortalities in the Control and Treatment cages respectively. Thereafter the weekly losses in the Treatment cage declined to between 16 and 35 individuals, while those in the Control remained between 34 and 44 for the rest of the trial. By the final weekly sampling on day 63, survival was 79.5% (1,192 of 1,500) in the Treatment cage against 68.8% (1,032 of 1,500) in the Control, a difference of 10.7 percentage points. Cumulative mortality over the full 70 days was 330 prawns in the Treatment cage and 510 in the Control, giving survival at harvest of 78.0% and 66.0% respectively — that is, the Control cage lost 1.55 times as many prawns as the Treatment cage over the same period and under the same husbandry.

Growth followed the same direction (Figure 10a,b; Table 3). The two cages were closely matched at stocking, with mean body weights of 0.62 ± 0.10 g (Control) and 0.62 ± 0.10 g (Treatment) and mean total lengths of 3.31 ± 0.21 cm and 3.30 ± 0.23 cm. The trajectories separated from week 2 onward and the gap widened steadily thereafter. At the final weekly sampling the mean body weight was 1.57 ± 0.29 g in the Treatment cage against 1.39 ± 0.24 g in the Control, an advantage of 0.18 g or 13.2%, and the mean total length was 4.47 ± 0.34 cm against 4.31 ± 0.31 cm, an advantage of 0.15 cm or 3.6%. Over the 63-day measured period this corresponds to a specific growth rate of 1.48% day⁻¹ in the Treatment cage against 1.28% day⁻¹ in the Control, and to an average daily gain of 0.0151 g day⁻¹ against 0.0122 g day⁻¹, an increase of 24.1%. In both cages SGR declined monotonically as the prawns grew, from 2.58% and 2.21% day⁻¹ in week 1 to 1.48% and 1.28% day⁻¹ in week 9, but the Treatment cage retained the higher value at every one of the nine sampling intervals.

Feed utilization differed more sharply than growth itself. Because the daily ration was recalculated weekly from the measured standing biomass, the Control cage — which carried fewer and smaller prawns — was offered less feed in absolute terms over the trial (2,776.3 g against 3,186.1 g), so the difference in feed efficiency reported here cannot be attributed to over-feeding of the Control. Nevertheless, the Control returned a larger share of the ration uneaten: cumulative recovered residue amounted to 15.0% of the feed offered, against 10.3% in the Treatment cage. Actual cumulative intake was therefore 2,561.7 g in the Treatment cage and 2,148.6 g in the Control, a difference of 19.2%, while the corresponding biomass gains were 944.4 g and 501.9 g, a difference of 88.2%. The resulting feed-conversion ratio, computed at the cage level from feed actually consumed, was 2.71 in the Treatment cage against 4.28 in the Control, a reduction of 36.6%. The weekly FCR series (Table 3) shows that this gap was present from the first week and widened over the trial: the Treatment FCR rose modestly from 2.00 to 2.71, whereas the Control FCR rose from 2.76 to 4.28.

Taken together, the higher survival and the higher individual weight compounded at the cage level. Standing biomass at the final weekly sampling was 1,873.4 g in the Treatment cage against 1,432.4 g in the Control — 520.4 g m⁻³ against 397.9 g m⁻³ of rearing water, or 312.2 g m⁻² against 238.7 g m⁻² of cage floor — an increase of 30.8% in harvestable biomass obtained from the same cage volume, the same stocking density, the same feed, and the same site, with 19.2% more feed actually consumed.

---

### Table 2. Water-quality summary over the 70-day grow-out.

*(caption แบบเต็ม: Table 2. Summary of the daily water-quality records of the Control and Treatment cages over the 70-day grow-out (one reading per cage per day at 12:00 h; n = 70 days per cage).)*

| Parameter | Control (mean ± SD) | Treatment (mean ± SD) | Control (min–max) | Treatment (min–max) | CV Control (%) | CV Treatment (%) | Days outside normal band <sup>1</sup> |
|---|---|---|---|---|---|---|---|
| Dissolved oxygen (mg L⁻¹) | 5.76 ± 0.56 | 6.32 ± 0.33 | 4.2–7.1 | 5.7–7.1 | 9.7 | 5.2 | 5 / 0 |
| pH | 7.50 ± 0.21 | 7.64 ± 0.10 | 6.9–8.0 | 7.4–7.9 | 2.8 | 1.3 | 0 / 0 |
| Water temperature (°C) | 28.31 ± 1.11 | 28.24 ± 1.12 | 25.5–30.1 | 25.6–30.2 | 3.9 | 4.0 | 0 / 0 |

<sup>1</sup> Number of days on which the daily reading fell outside the normal band defined in Section 3.2 (DO ≥ 5.0 mg L⁻¹; pH 7.0–8.3; water temperature 26–30 °C), reported as Control / Treatment. For DO this corresponds to entry into the surveillance band; no reading in either cage reached the critical threshold of 4.0 mg L⁻¹ at the daily reading time.

---

### Table 3. Weekly culture performance of the Control and Treatment cages.

*(caption แบบเต็ม: Table 3. Survival, growth, and feed-utilization indices of M. lanchesteri reared in the Control and Treatment cages at each of the ten weekly sampling points. Mean body weight and mean total length are given as mean ± SD of the 30 prawns individually measured in each cage at each sampling. FCR is computed at the cage level from feed actually consumed.)*

| Week (day) | Group | Survivors (ind.) | Survival (%) | Mean weight (g) | Mean length (cm) | Biomass (g) | SGR (% day⁻¹) | ADG (g day⁻¹) | Cum. feed consumed (g) | FCR |
|---|---|---|---|---|---|---|---|---|---|---|
| 0 (0) | Control | 1,500 | 100.0 | 0.620 ± 0.101 | 3.31 ± 0.21 | 930.5 | — | — | 0.0 | — |
| | Treatment | 1,500 | 100.0 | 0.619 ± 0.095 | 3.30 ± 0.23 | 929.0 | — | — | 0.0 | — |
| 1 (7) | Control | 1,397 | 93.1 | 0.724 ± 0.126 | 3.47 ± 0.22 | 1,011.9 | 2.21 | 0.0149 | 224.6 | 2.76 |
| | Treatment | 1,411 | 94.1 | 0.742 ± 0.142 | 3.45 ± 0.20 | 1,046.5 | 2.58 | 0.0175 | 234.7 | 2.00 |
| 2 (14) | Control | 1,315 | 87.7 | 0.830 ± 0.107 | 3.65 ± 0.23 | 1,091.0 | 2.08 | 0.0150 | 470.8 | 2.93 |
| | Treatment | 1,369 | 91.3 | 0.862 ± 0.135 | 3.70 ± 0.22 | 1,180.5 | 2.36 | 0.0174 | 491.5 | 1.95 |
| 3 (21) | Control | 1,273 | 84.9 | 0.930 ± 0.144 | 3.81 ± 0.22 | 1,183.9 | 1.93 | 0.0147 | 731.9 | 2.89 |
| | Treatment | 1,341 | 89.4 | 0.978 ± 0.113 | 3.81 ± 0.19 | 1,311.9 | 2.18 | 0.0171 | 790.9 | 2.07 |
| 4 (28) | Control | 1,230 | 82.0 | 1.028 ± 0.221 | 3.85 ± 0.33 | 1,264.9 | 1.81 | 0.0146 | 980.3 | 2.93 |
| | Treatment | 1,314 | 87.6 | 1.094 ± 0.151 | 4.00 ± 0.20 | 1,438.0 | 2.03 | 0.0170 | 1,084.9 | 2.13 |
| 5 (35) | Control | 1,193 | 79.5 | 1.115 ± 0.147 | 4.02 ± 0.22 | 1,329.8 | 1.67 | 0.0141 | 1,241.6 | 3.11 |
| | Treatment | 1,290 | 86.0 | 1.204 ± 0.186 | 4.11 ± 0.24 | 1,553.6 | 1.90 | 0.0167 | 1,402.1 | 2.25 |
| 6 (42) | Control | 1,149 | 76.6 | 1.199 ± 0.226 | 4.08 ± 0.32 | 1,377.7 | 1.57 | 0.0138 | 1,479.0 | 3.31 |
| | Treatment | 1,255 | 83.7 | 1.312 ± 0.233 | 4.21 ± 0.28 | 1,647.0 | 1.79 | 0.0165 | 1,690.8 | 2.36 |
| 7 (49) | Control | 1,106 | 73.7 | 1.276 ± 0.172 | 4.23 ± 0.24 | 1,410.9 | 1.47 | 0.0134 | 1,721.4 | 3.58 |
| | Treatment | 1,235 | 82.3 | 1.412 ± 0.321 | 4.33 ± 0.35 | 1,743.4 | 1.68 | 0.0162 | 1,997.4 | 2.45 |
| 8 (56) | Control | 1,072 | 71.5 | 1.338 ± 0.232 | 4.26 ± 0.29 | 1,434.3 | 1.37 | 0.0128 | 1,935.7 | 3.84 |
| | Treatment | 1,219 | 81.3 | 1.499 ± 0.244 | 4.40 ± 0.26 | 1,827.3 | 1.58 | 0.0157 | 2,274.7 | 2.53 |
| 9 (63) | Control | 1,032 | 68.8 | 1.388 ± 0.241 | 4.31 ± 0.31 | 1,432.4 | 1.28 | 0.0122 | 2,148.6 | 4.28 |
| | Treatment | 1,192 | 79.5 | 1.572 ± 0.295 | 4.47 ± 0.34 | 1,873.4 | 1.48 | 0.0151 | 2,561.7 | 2.71 |

*หมายเหตุ: ถ้าตารางนี้ยาวเกินไปสำหรับเนื้อบทความ ให้ย้ายไปเป็น Table A1 ใน Appendix แล้วแทนที่ด้วยตารางสรุปเฉพาะ Week 0 และ Week 9 (initial vs final) ในเนื้อความ*

---

### Figure 10 (ไฟล์: `Figure10_growth_survival.png`)

**Figure 10.** Culture performance of *M. lanchesteri* over the ten weekly sampling points (weeks 0–9) in the Control cage and in the cage equipped with the IoT monitoring-and-control system (Treatment): (**a**) mean body weight; (**b**) mean total length; (**c**) survival. Error bars in (**a**) and (**b**) are the standard deviations of the 30 prawns individually measured in each cage at each sampling.

---

## ส่วนที่ 4 — เช็กลิสต์ก่อนส่ง

- [ ] เปลี่ยนหัวข้อ `4.1. -` เป็น `4.1. Culture Performance` และลบย่อหน้า `-.`
- [ ] ลบเนื้อหา template ที่เหลือ (ย่อหน้า “3.1. System Circuit Design”, “3.1.1. Subsubsection”, bulleted list, ตาราง Title 1/Title 2 ทั้งหมด) ที่อยู่ท้ายบทความ
- [ ] ตัดสินใจประเด็น day 63 vs day 70 (ส่วนที่ 2 ข้อ 1)
- [ ] ตรวจว่าเลข Table 2, Table 3, Figure 10 ไม่ชนกับหัวข้อ 4.2–4.4 ที่จะเขียนต่อ
- [ ] เพิ่มบรรทัดใน Supplementary Materials: “Table S1: daily log; Table S2: weekly sampling; Table S3: individual measurements (n = 600)”
- [ ] ตรวจ Abstract ให้มีตัวเลขชี้ขาดจาก 4.1: survival 79.5% vs 68.8%, FCR 2.71 vs 4.28, biomass +30.8%
- [ ] ย้ายประโยคเชิงตีความทั้งหมด (คำว่า *because, owing to, attributable to*) ออกจาก 4.1 ไปยัง Discussion
