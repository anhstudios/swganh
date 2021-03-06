-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:19
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.skills_schematicsgranted
DROP TABLE IF EXISTS `skills_schematicsgranted`;
CREATE TABLE IF NOT EXISTS `skills_schematicsgranted` (
  `skill_id` int(11) unsigned NOT NULL,
  `schem_group_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`skill_id`,`schem_group_id`),
  KEY `fk_skills_schemgranted_sg_sg` (`schem_group_id`),
  CONSTRAINT `fk_skills_schemgranted_sg_sg` FOREIGN KEY (`schem_group_id`) REFERENCES `schematic_groups` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_skills_schemgranted_skill_skill` FOREIGN KEY (`skill_id`) REFERENCES `skills` (`skill_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.skills_schematicsgranted: ~281 rows (approximately)
/*!40000 ALTER TABLE `skills_schematicsgranted` DISABLE KEYS */;
INSERT INTO `skills_schematicsgranted` (`skill_id`, `schem_group_id`) VALUES
	(11, 1),
	(18, 2),
	(51, 3),
	(73, 3),
	(284, 4),
	(285, 5),
	(286, 6),
	(358, 7),
	(358, 8),
	(360, 9),
	(362, 10),
	(567, 11),
	(567, 12),
	(567, 13),
	(382, 14),
	(383, 15),
	(384, 16),
	(385, 17),
	(377, 18),
	(376, 19),
	(378, 20),
	(379, 21),
	(380, 22),
	(381, 23),
	(377, 24),
	(386, 25),
	(387, 26),
	(388, 27),
	(389, 28),
	(77, 29),
	(78, 30),
	(79, 31),
	(80, 32),
	(73, 33),
	(74, 34),
	(75, 35),
	(76, 36),
	(73, 37),
	(357, 38),
	(357, 39),
	(435, 40),
	(436, 41),
	(437, 42),
	(438, 43),
	(439, 44),
	(440, 45),
	(441, 46),
	(442, 47),
	(443, 48),
	(444, 49),
	(445, 50),
	(446, 51),
	(433, 52),
	(433, 53),
	(433, 54),
	(314, 56),
	(314, 57),
	(300, 58),
	(300, 59),
	(359, 60),
	(361, 61),
	(477, 62),
	(478, 63),
	(479, 64),
	(480, 65),
	(477, 66),
	(478, 67),
	(479, 68),
	(480, 69),
	(473, 70),
	(474, 71),
	(475, 72),
	(476, 73),
	(471, 74),
	(471, 75),
	(471, 76),
	(471, 77),
	(485, 78),
	(486, 79),
	(487, 80),
	(488, 81),
	(472, 82),
	(472, 83),
	(472, 84),
	(472, 85),
	(420, 86),
	(421, 87),
	(422, 88),
	(423, 89),
	(416, 90),
	(417, 91),
	(418, 92),
	(419, 93),
	(424, 94),
	(425, 95),
	(426, 96),
	(427, 97),
	(414, 98),
	(414, 99),
	(414, 100),
	(454, 101),
	(455, 102),
	(456, 103),
	(457, 104),
	(452, 105),
	(452, 106),
	(452, 107),
	(462, 108),
	(463, 109),
	(464, 110),
	(465, 111),
	(51, 112),
	(66, 113),
	(566, 114),
	(68, 115),
	(51, 116),
	(66, 117),
	(68, 118),
	(52, 119),
	(316, 120),
	(65, 121),
	(67, 122),
	(317, 124),
	(301, 125),
	(67, 126),
	(68, 127),
	(316, 128),
	(317, 129),
	(301, 130),
	(316, 131),
	(316, 132),
	(317, 133),
	(315, 135),
	(316, 136),
	(317, 137),
	(301, 138),
	(315, 140),
	(317, 141),
	(301, 142),
	(395, 143),
	(405, 144),
	(406, 145),
	(407, 146),
	(408, 147),
	(321, 148),
	(322, 149),
	(323, 150),
	(324, 151),
	(320, 152),
	(319, 153),
	(326, 154),
	(328, 155),
	(334, 156),
	(335, 157),
	(31, 158),
	(31, 159),
	(46, 160),
	(48, 161),
	(1006, 163),
	(1007, 164),
	(1008, 165),
	(1009, 166),
	(994, 167),
	(995, 168),
	(996, 169),
	(997, 170),
	(992, 171),
	(998, 172),
	(999, 173),
	(1000, 174),
	(1001, 175),
	(1002, 176),
	(1003, 177),
	(1004, 178),
	(1005, 179),
	(466, 180),
	(467, 181),
	(468, 182),
	(469, 183),
	(453, 184),
	(453, 185),
	(453, 186),
	(363, 187),
	(364, 188),
	(365, 189),
	(366, 190),
	(395, 191),
	(397, 192),
	(398, 193),
	(399, 194),
	(400, 195),
	(395, 196),
	(401, 197),
	(402, 198),
	(403, 199),
	(404, 200),
	(396, 201),
	(396, 202),
	(566, 203),
	(576, 204),
	(577, 205),
	(578, 206),
	(579, 207),
	(566, 208),
	(577, 209),
	(579, 210),
	(576, 211),
	(578, 212),
	(577, 213),
	(579, 214),
	(37, 215),
	(38, 216),
	(39, 217),
	(40, 218),
	(71, 219),
	(71, 220),
	(71, 221),
	(72, 222),
	(72, 223),
	(281, 224),
	(396, 226),
	(576, 227),
	(577, 228),
	(578, 229),
	(415, 230),
	(434, 231),
	(458, 232),
	(459, 233),
	(460, 234),
	(461, 235),
	(481, 236),
	(482, 237),
	(483, 238),
	(484, 239),
	(509, 240),
	(509, 241),
	(516, 242),
	(518, 243),
	(523, 244),
	(524, 245),
	(525, 246),
	(526, 247),
	(682, 248),
	(688, 249),
	(986, 249),
	(688, 250),
	(690, 251),
	(864, 251),
	(691, 252),
	(866, 252),
	(692, 253),
	(693, 254),
	(707, 255),
	(745, 255),
	(709, 256),
	(747, 256),
	(868, 256),
	(710, 257),
	(748, 257),
	(870, 257),
	(711, 258),
	(749, 258),
	(712, 259),
	(750, 259),
	(726, 260),
	(764, 260),
	(728, 261),
	(766, 261),
	(872, 261),
	(729, 262),
	(767, 262),
	(874, 262),
	(730, 263),
	(768, 263),
	(731, 264),
	(769, 264),
	(861, 265),
	(862, 266),
	(986, 267),
	(993, 268),
	(475, 269);
/*!40000 ALTER TABLE `skills_schematicsgranted` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
